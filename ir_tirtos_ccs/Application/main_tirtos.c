//*****************************************************************************
//  SENSOR CONTROLLER STUDIO EXAMPLE: ADC WINDOW MONITOR FOR LAUNCHPAD
//  Operating system: TI-RTOS
//
//  The Sensor Controller is used to sample a single ADC channel and monitor
//  the value. The Sensor Controller updates a bit-vector that indicates
//  whether the ADC value is:
//  - Below a configurable low threshold
//  - Above a configurable high threshold
//
//  The Sensor Controller notifies the application when the bit-vector changes
//  (triggering scTaskAlertCallback()), and the application sets the LEDs as
//  follows:
//  - Green LED is set whenever the ADC value is below the low threshold
//  - Red LED is set whenever the ADC value is above the high threshold
//
//
//  Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//****************************************************************************/
/* XDCtools Header files */
#/* For usleep() */
#include <unistd.h>
/* Standard C Libraries */
#include <stdlib.h>

/* TI Drivers */
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

// DriverLib
#include <driverlib/aon_batmon.h>

/* Driverlib Header files */
#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)

/* Board Header files */
#include "Board.h"
#include "smartrf_settings/smartrf_settings.h"

#include "ex_include_tirtos.h"
#if(sensor==ir)
#include "TSOP38140sce/scif_tsop38140.h"
#elif(sensor==ws)
#include "HX711sce/scif_hx711.h"
#endif


#define BV(n)               (1 << (n))

#define ADVLEN              3

// Display error message if the SCIF driver has been generated with incorrect operating system setting
#ifndef SCIF_OSAL_TIRTOS_H
    #error "SCIF driver has incorrect operating system configuration for this example. Please change to 'TI-RTOS' in the Sensor Controller Studio project panel and re-generate the driver."
#endif

// Display error message if the SCIF driver has been generated with incorrect target chip package
#ifndef SCIF_TARGET_CHIP_PACKAGE_QFN32_4X4_RSM
    #error "SCIF driver has incorrect target chip package configuration for this example. Please change to 'QFN48 7x7 RGZ' in the Sensor Controller Studio project panel and re-generate the driver."
#endif

// Task data
Task_Struct myTask;
Char myTaskStack[1024];

static RF_Object rfObject;
static RF_Handle rfHandle;

uint8_t adcN = 4;
uint8_t advLen = ADVLEN;
uint8_t beaconData[ADVLEN+2] = {0x00, 0x00, 0x00, 0x00, 0x00};

typedef union {
    uint8_t value;
    struct {
        //unsigned char CHAN:2;
        unsigned char RND:3;
        unsigned char SEQ:2;
        unsigned char BUZZ0:1;
        unsigned char BUZZ1:1;
        unsigned char TYPE:1;
    };
} sqn_t;

typedef struct {
//    uint8 TXFLEN;
//    adv_header_t HEADER;
//    uint8 Address[ DEVICE_ADDR_LEN ];
//    uint8_t LEN;
    sqn_t SQN;
#if(sensor==23)
    uint8_t BATT;
    int16_t ADC;
#elif(sensor==711)
    int16_t ADC;
    uint8_t BATT;
#endif
    int16_t ADC0;
    int16_t ADC1;
    uint8_t BATT0;
    uint8_t SENSOR:1;
    uint8_t FLAG:1;
//    uint8_t xTXFLEN;
//    uint8_t xLength;
//    uint8_t xLEN;
//    uint8_t xRFTXFWP;
    uint8_t dCOUNT;
    uint8_t bCOUNT;
} adv_data_t;

static adv_data_t ADV_DATA;
// Semaphore used to wait for Sensor Controller task ALERT event
static Semaphore_Struct semScTaskAlert;

void scCtrlReadyCallback(void) {

} // scCtrlReadyCallback


void scTaskAlertCallback(void) {

    // Wake up the OS task
    Semaphore_post(Semaphore_handle(&semScTaskAlert));

} // scTaskAlertCallback



/*
PIN_Config pLedPinTable[] = {
    Board_GLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_RLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};
PIN_State ledPinState;
*/

void taskFxn(UArg a0, UArg a1) {
//    PIN_Handle hLedPins;

    // Enable LED pins
//    hLedPins = PIN_open(&ledPinState, pLedPinTable);

    // Initialize the Sensor Controller
    scifOsalInit();
    scifOsalRegisterCtrlReadyCallback(scCtrlReadyCallback);
    scifOsalRegisterTaskAlertCallback(scTaskAlertCallback);
    scifInit(&scifTsop38140DriverSetup);
    IOCIODrvStrengthSet(IOID_9, IOC_CURRENT_4MA, PIN_DRVSTR_MAX);
    IOCIODrvStrengthSet(IOID_0, IOC_CURRENT_8MA, PIN_DRVSTR_MAX);
    scifTsop38140StartRtcTicksNow(0x00060000);  //6s

    scifStartTasksNbl(BV(SCIF_TSOP38140_IRLED_TASK_ID));

#if(sensor==23)
    ADV_DATA.SQN.TYPE = 0;
    tsop38140Start();
#elif(sensor==711)
    ADV_DATA.SQN.TYPE = 1;
    hx711Start();
#endif

    uint32_t batt;
    RF_Params rfParams;
    RF_Params_init(&rfParams);

    /* Request access to the radio */
    rfHandle = RF_open(&rfObject, &RF_ble, (RF_RadioSetup*)&RF_cmdBleRadioSetup, &rfParams);

    // Main loop
    while (1) {

        // Wait for an ALERT callback
        Semaphore_pend(Semaphore_handle(&semScTaskAlert), BIOS_WAIT_FOREVER);

        // Clear the ALERT interrupt source
        scifClearAlertIntSource();

#if(sensor==ir)
        ADV_DATA.SQN.BUZZ0 = scifTsop38140TaskData.irled.output.buzz;
        ADV_DATA.SQN.BUZZ1 = scifTsop38140TaskData.irled.state.buzz;

        if(scifTsop38140TaskData.irled.state.bCount >= scifTsop38140TaskData.irled.cfg.bCount) {
            scifTsop38140TaskData.irled.state.bCount = 0;
#elif(sensor==ws)
            ADV_DATA.SQN.BUZZ0 = scifHx711TaskData.adc24bit.output.buzz;
            ADV_DATA.SQN.BUZZ1 = scifHx711TaskData.adc24bit.state.buzz;

            //hx711adcAvg(&ADV_DATA.ADC);
            *((uint16_t *)&beaconData[2]) = scifHx711TaskData.adc24bit.output.adcVal;

        if(scifHx711TaskData.adc24bit.state.bCount >= scifHx711TaskData.adc24bit.cfg.bCount) {
            scifHx711TaskData.adc24bit.state.bCount = 0;
#endif
        // Battery voltage (bit 10:8 - integer, but 7:0 fraction)
        batt = AONBatMonBatteryVoltageGet();
        //batt = (batt * 125) >> 5; // convert V to mV

#if(sensor==ir)
        beaconData[2] = (batt >> 3) - 0x40;
        advLen = ADVLEN;
        } else
            advLen = ADVLEN - 1;
#elif(sensor==ws)
        beaconData[4] = (batt >> 3) - 0x40;
        advLen = ADVLEN + 2;
        } else
            advLen = ADVLEN + 1;
#endif
        beaconData[0] = advLen - 1;
        //set adv data
        RF_cmdAdvNc0.pParams->pAdvData = (uint8_t*)beaconData;
        RF_cmdAdvNc0.pParams->advLen = advLen;

        /* Get current time */
        //time = RF_getCurrentTime();
        //for (advCount=0;  advCount < ABEACON_COUNT; advCount++) // aBeacon packet
        for (uint8_t n = 0;  n < adcN; n++) // aBeacon packet
        {
          beaconData[1] = ADV_DATA.SQN.value;

          //ADV_DATA.ADC = scifHx711TaskData.adc24bit.output.pSamples[n];
          //set time for next packet
          //time += PACKET_INTERVAL;

          /* Set absolute TX time to utilize automatic power management */
          //RF_cmdAdvNc0.startTime = time;
          usleep(50000);
          /* Send packet */
          RF_EventMask result = RF_runCmd(rfHandle, (RF_Op*)&RF_cmdAdvNc0, RF_PriorityNormal, NULL, 0);
          if (!(result & RF_EventLastCmdDone))
          {
            /* Error */
            while(1);
          }
          ADV_DATA.SQN.RND++;
        }
      //RF_close(rfHandle)
        ADV_DATA.SQN.SEQ++;
#if(sensor==ir)
        scifTsop38140TaskData.irled.state.exit = 0;
#elif(sensor==ws)
        //hx711adcN();
        scifHx711TaskData.adc24bit.state.exit = 0;
#endif
        // Acknowledge the alert event
        scifAckAlertEvents();
    }

} // taskFxn




int main(void) {
    Task_Params taskParams;

    // Initialize the PIN driver
    PIN_init(BoardGpioInitTable);

    // Configure the OS task
    Task_Params_init(&taskParams);
    taskParams.stack = myTaskStack;
    taskParams.stackSize = sizeof(myTaskStack);
    taskParams.priority = 2;
    Task_construct(&myTask, taskFxn, &taskParams, NULL);

    // Create the semaphore used to wait for Sensor Controller ALERT events
    Semaphore_Params semParams;
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    Semaphore_construct(&semScTaskAlert, 0, &semParams);

    // Start TI-RTOS
    BIOS_start();
    return 0;

} // main
