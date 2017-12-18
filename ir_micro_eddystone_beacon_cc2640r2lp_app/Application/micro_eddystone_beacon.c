/*********************************************************************
 * INCLUDES
 */
#include <string.h>
#include <stdlib.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/display/Display.h>

#include "bcomdef.h"

#include "board.h"

// DriverLib
#include <driverlib/aon_batmon.h>
#include <irws_tirtos.h>
#include <urfc.h>
#include <uble.h>
#include <ugap.h>

#include <util.h>
#ifdef USE_ICALL
  #include <icall.h>
#endif /* USE_ICALL */

#include <gap.h>
#include "micro_eddystone_beacon.h"
#include "irws_tirtos.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// Advertising interval (units of 0.625 millisec)
#define DEFAULT_ADVERTISING_INTERVAL          160 // 100 ms

// Task configuration
#define UEB_TASK_PRIORITY                     1

#ifndef UEB_TASK_STACK_SIZE
#define UEB_TASK_STACK_SIZE                   600
#endif

// Internal Events for RTOS application
#ifdef USE_ICALL
#define UEB_ICALL_EVT                           ICALL_MSG_EVENT_ID // Event_Id_31
#else /* !USE_ICALL */
#define UEB_ICALL_EVT                           Event_Id_NONE
#endif /* USE_ICALL */
#define UEB_QUEUE_EVT                           UTIL_QUEUE_EVENT_ID // Event_Id_30
#define UEB_EVT_SCIF_ALERT                      0x0001
#define UEB_EVT_MICROBLESTACK                   0x0002

#define UEB_ALL_EVENTS                          (UEB_ICALL_EVT     | \
                                                UEB_QUEUE_EVT      | \
                                                UEB_EVT_SCIF_ALERT | \
                                                UEB_EVT_MICROBLESTACK)

// Pre-generated Random Static Address
#if(sensor==23)
#define UEB_PREGEN_RAND_STATIC_ADDR    {0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc}
#elif(sensor==711)
#define UEB_PREGEN_RAND_STATIC_ADDR    {0x88, 0x88, 0x99, 0xaa, 0xbb, 0xcc}
#endif

/*********************************************************************
 * TYPEDEFS
 */

// App to App event
typedef struct {
  uint16 event;
  adv_data_t *data;
} uebEvt_t;


/*********************************************************************
 * GLOBAL VARIABLES
 */
// Display Interface
Display_Handle dispHandle = NULL;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

#ifdef USE_ICALL
// Event globally used to post local events and pend on local events.
static ICall_SyncHandle syncEvent;
#else
// Event globally used to post local events and pend on local events.
static Event_Handle syncEvent;
#endif /* USE_ICALL */

// Queue object used for app messages
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

// Task configuration
Task_Struct uebTask;
Char uebTaskStack[UEB_TASK_STACK_SIZE];

// Broadcaster state
static bool uebBcastActive;

adv_data_t* pFrame;
// Pointer to application callback
//keysPressedCB_t appKeyChangeHandler_st = NULL;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void MicroEddystoneBeacon_init(void);
static void MicroEddystoneBeacon_taskFxn(UArg a0, UArg a1);

static void MicroEddystoneBeacon_scifAlertHandler(adv_data_t *advData);

static void MicroEddystoneBeacon_processAppMsg(uebEvt_t *pMsg);

static void MicroEddystoneBeacon_bcast_stateChangeCB(ugapBcastState_t newState);
static void MicroEddystoneBeacon_bcast_advPrepareCB(void);
static void MicroEddystoneBeacon_bcast_advDoneCB(bStatus_t status);

static bStatus_t MicroEddystoneBeacon_enqueueMsg(uint16 event, adv_data_t *data);

static void uBLEStack_eventProxy(void);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      MicroEddystoneBeacon_createTask
 *
 * @brief   Task creation function for the Micro Eddystone Beacon.
 *
 * @param   None.
 *
 * @return  None.
 */
void MicroEddystoneBeacon_createTask(void)
{
  Task_Params taskParams;

  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = uebTaskStack;
  taskParams.stackSize = UEB_TASK_STACK_SIZE;
  taskParams.priority = UEB_TASK_PRIORITY;

  Task_construct(&uebTask, MicroEddystoneBeacon_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      MicroEddystoneBeacon_init
 *
 * @brief   Initialization function for the Micro Eddystone Beacon App
 *          Task. This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notification ...).
 *
 * @param   none
 *
 * @return  none
 */
static void MicroEddystoneBeacon_init(void)
{
#if defined(FEATURE_STATIC_ADDR)
  uint8 staticAddr[] = UEB_PREGEN_RAND_STATIC_ADDR;
#endif /* FEATURE_STATIC_ADDR */
  ugapBcastCBs_t bcastCBs = {
    MicroEddystoneBeacon_bcast_stateChangeCB,
    MicroEddystoneBeacon_bcast_advPrepareCB,
    MicroEddystoneBeacon_bcast_advDoneCB };

  // Create an RTOS event used to wake up this application to process events.
  syncEvent = Event_create(NULL, NULL);

  // Create an RTOS queue for message from profile to be sent to app.
  appMsgQueue = Util_constructQueue(&appMsg);

  // Setup keycallback for keys
  Scif_start(MicroEddystoneBeacon_scifAlertHandler);

  /* Initialize Micro BLE Stack */
#if defined(FEATURE_STATIC_ADDR)
  uble_stackInit(UBLE_ADDRTYPE_STATIC, staticAddr, uBLEStack_eventProxy,
               RF_TIME_CRITICAL);
#else  /* FEATURE_STATIC_ADDR */
  uble_stackInit(UBLE_ADDRTYPE_PUBLIC, NULL, uBLEStack_eventProxy, RF_TIME_CRITICAL);
#endif /* FEATURE_STATIC_ADDR */

  /* Initilaize Micro GAP Broadcaster Role */
  ugap_bcastInit(&bcastCBs);
}

/*********************************************************************
 * @fn      MicroEddystoneBeacon_processEvent
 *
 * @brief   Application task entry point for the Micro Eddystone Beacon.
 *
 * @param   none
 *
 * @return  none
 */
static void MicroEddystoneBeacon_taskFxn(UArg a0, UArg a1)
{
  // Initialize application
  MicroEddystoneBeacon_init();

  for (;;)
  {
    uint32 events;

    // Waits for an event to be posted associated with the calling thread.
    // Note that an event associated with a thread is posted when a
    // message is queued to the message receive queue of the thread
#ifdef USE_ICALL
    events = Event_pend(syncEvent, Event_Id_NONE, UEB_ALL_EVENTS,
                        ICALL_TIMEOUT_FOREVER);
#else /* !USE_ICALL */
    events = Event_pend(syncEvent, Event_Id_NONE, UEB_ALL_EVENTS,
                        BIOS_WAIT_FOREVER);
#endif /* USE_ICALL */

    if (events | UEB_QUEUE_EVT)
    {
      // If RTOS queue is not empty, process app message.
      while (!Queue_empty(appMsgQueue))
      {
        uebEvt_t *pMsg = (uebEvt_t *) Util_dequeueMsg(appMsgQueue);
        if (pMsg)
        {
          // Process message.
          MicroEddystoneBeacon_processAppMsg(pMsg);

          // Free the space from the message.
#ifdef USE_ICALL
          ICall_free(pMsg);
#else /* USE_ICALL */
          free(pMsg);
#endif /* USE_ICALL */
        }
      }
    }
  }
}

/*********************************************************************
 * @fn      MicroEddystoneBeacon_handleAlert
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 KEY_LEFT
 *                 KEY_RIGHT
 *
 * @return  none
 */
static void MicroEddystoneBeacon_handleAlert(adv_data_t *data) {
  pFrame = data;
  uble_setParameter(UBLE_PARAM_ADVDATA, pFrame->LEN + 1, (void*) pFrame);
  ugap_bcastStart(2);
}

/*********************************************************************
 * @fn      MicroEddystoneBeacon_keyChangeHandler
 *
 * @brief   Key event handler function
 *
 * @param   a0 - ignored
 *
 * @return  none
 */
void MicroEddystoneBeacon_scifAlertHandler(adv_data_t *advData)
{
  MicroEddystoneBeacon_enqueueMsg(UEB_EVT_SCIF_ALERT, advData);
}

/*********************************************************************
 * @fn      MicroEddystoneBeacon_processAppMsg
 *
 * @brief   Process an incoming event message.
 *
 * @param   pMsg - message to process
 *
 * @return  None.
 */
static void MicroEddystoneBeacon_processAppMsg(uebEvt_t *pMsg)
{
  switch (pMsg->event)
  {
  case UEB_EVT_SCIF_ALERT:
    MicroEddystoneBeacon_handleAlert(pMsg->data);
    break;

  case UEB_EVT_MICROBLESTACK:
    uble_processMsg();
    break;

  default:
    // Do nothing.
    break;
  }
}

/*********************************************************************
 * @fn      MicroEddystoneBeacon_bcast_stateChange_CB
 *
 * @brief   Callback from Micro Broadcaster indicating a state change.
 *
 * @param   newState - new state
 *
 * @return  None.
 */
static void MicroEddystoneBeacon_bcast_stateChangeCB(ugapBcastState_t newState)
{
  switch (newState)
  {
  case UGAP_BCAST_STATE_INITIALIZED:
    uebBcastActive = FALSE;
    {
      // Parameter containers
      uint16 param16; /* 16-bit parameter */

      // Setup broadcaster duty cycle
//    ugap_bcastSetDuty(100, 50); /* OnTime: 10 sec, OffTime: 5 sec */

      // Setup adv interval
      param16 = DEFAULT_ADVERTISING_INTERVAL;
      uble_setParameter(UBLE_PARAM_ADVINTERVAL, sizeof(uint16), &param16);

      //uint8  param8;  /* 8-bit parameter */
      // Setup adv channel map
//    param8 = UB_ADV_CHAN_38 | UB_ADV_CHAN_39; /* Use only channels 38 & 39 */
//    uble_setParameter(UB_PARAM_ADVCHANMAP, sizeof(uint8), &param8);
    }
    // Start advertising
    //MicroEddystoneBeacon_startBroadcast();
    break;

  case UGAP_BCAST_STATE_IDLE:
    uebBcastActive = FALSE;
    break;

  case UGAP_BCAST_STATE_ADVERTISING:
    uebBcastActive = TRUE;
    break;

  case UGAP_BCAST_STATE_WAITING:
    break;

  default:
    break;
  }
}

/*********************************************************************
 * @fn      MicroEddystoneBeacon_bcast_advPrepareCB
 *
 * @brief   Callback from Micro Broadcaster notifying that the next
 *          advertising event is about to start so it's time to update
 *          the adv payload.
 *
 * @param   None.
 *
 * @return  None.
 */
static void MicroEddystoneBeacon_bcast_advPrepareCB(void)
{
  uint8 timeToAdv = 0;

  // Disable ADV_PREPARE notification from the next Adv
  uble_setParameter(UBLE_PARAM_TIMETOADV, sizeof(timeToAdv), &timeToAdv);
}

/*********************************************************************
 * @fn      MicroEddystoneBeacon_bcast_advDoneCB
 *
 * @brief   Callback from Micro Broadcaster notifying that an
 *          advertising event has been done.
 *
 * @param   status - How the last event was done. SUCCESS or FAILURE.
 *
 * @return  None.
 */
static void MicroEddystoneBeacon_bcast_advDoneCB(bStatus_t status)
{
    pFrame->SQN.RND++;
}


/*********************************************************************
 * @fn      MicroEddystoneBeacon_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event - message event.
 * @param   data - message data.
 *
 * @return  TRUE or FALSE
 */
static bStatus_t MicroEddystoneBeacon_enqueueMsg(uint16 event, adv_data_t *data)
{
  uebEvt_t *pMsg;

  // Create dynamic pointer to message.
#ifdef USE_ICALL
  if ((pMsg = (uebEvt_t*) ICall_malloc(sizeof(uebEvt_t))))
#else /* USE_ICALL */
  if ((pMsg = (uebEvt_t*) malloc(sizeof(uebEvt_t))))
#endif /* USE_ICALL */
  {
    pMsg->event = event;
    pMsg->data = data;

    // Enqueue the message.
    return Util_enqueueMsg(appMsgQueue, syncEvent, (uint8_t *) pMsg);
  }

  return FALSE;
}


/*********************************************************************
 * @fn      uBLEStack_eventProxy
 *
 * @brief   Post an event to the application so that a Micro BLE Stack internal
 *          event is processed by Micro BLE Stack later in the application
 *          task's context.
 *
 * @param   None
 *
 * @return  None
 */
void uBLEStack_eventProxy()
{
  MicroEddystoneBeacon_enqueueMsg(UEB_EVT_MICROBLESTACK, NULL);
}

/*********************************************************************
 *********************************************************************/
