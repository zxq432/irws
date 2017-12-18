/*
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/***** Includes *****/
/* For usleep() */
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

/***** Defines *****/
#define TX_TASK_STACK_SIZE 1024
#define TX_TASK_PRIORITY   2

/* TX Configuration */
#define PACKET_INTERVAL                 50*4000   //50 ms @ 4 MHz
#define BBEACON_DELAY                   4000*4000 //4 seconds @4 MHz
#define ABEACON_COUNT                   6
#define ADV_PERIOD                      (6000 * 4000) - ((PACKET_INTERVAL * ABEACON_COUNT) + BBEACON_DELAY)
#define ADVLEN                          6

/* Do power measurement */
//#define POWER_MEASUREMENT

/***** Prototypes *****/



/***** Variable declarations *****/
static RF_Object rfObject;
static RF_Handle rfHandle;

/* Pin driver handle */
static PIN_Handle ledPinHandle;
static PIN_State ledPinState;

static uint8_t advCount = 1;

static char aBeaconData[ADVLEN] =
{
  0xAA,
  0x55,
  0xAA,
  0x55,
  0xAA,
  0x55,
};

/*
 * Application LED pin configuration table:
 *   - All LEDs board LEDs are off.
 */
PIN_Config pinTable[] =
{
    Board_PIN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#if defined __CC1352R1_LAUNCHXL_BOARD_H__
    Board_DIO30_RFSW | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
#ifdef POWER_MEASUREMENT
#if !defined(__CC1352R1_LAUNCHXL_BOARD_H__) && !defined(__CC26X2R1_LAUNCHXL_BOARD_H__)
    CC2640R2_RSM_DIO30_SWPWR | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
#endif
    PIN_TERMINATE
};

/***** Function definitions *****/

void *mainThread(void *arg0)
{
    uint32_t batt;
    uint32_t time;
    RF_Params rfParams;
    RF_Params_init(&rfParams);

    /* Open LED pins */
    ledPinHandle = PIN_open(&ledPinState, pinTable);
    if (ledPinHandle == NULL)
    {
        while(1);
    }
    sleep(1);
#ifdef POWER_MEASUREMENT
    /* Shutdown external flash */
    Board_shutDownExtFlash();
#if !defined(__CC1352R1_LAUNCHXL_BOARD_H__) && !defined(__CC26X2R1_LAUNCHXL_BOARD_H__)
    /* Route out PA active pin to Board_DIO30_SWPWR */
    PINCC26XX_setMux(ledPinHandle, Board_DIO30_SWPWR, PINCC26XX_MUX_RFC_GPO1);
#endif
#endif

    /* Request access to the radio */
    rfHandle = RF_open(&rfObject, &RF_ble, (RF_RadioSetup*)&RF_cmdBleRadioSetup, &rfParams);

    /* Get current time */
    time = RF_getCurrentTime();
    while(1)
    {
        // Battery voltage (bit 10:8 - integer, but 7:0 fraction)
        batt = AONBatMonBatteryVoltageGet();
        //batt = (batt * 125) >> 5; // convert V to mV
        //aBeaconData[0] = HI_UINT16(batt);
        //aBeaconData[1] = LO_UINT16(batt);
        aBeaconData[0] = (batt >> 3) & 0xFF;

        if (advCount == 1) //first aBeacon packet
        {
          //set adv data
          //RF_cmdAdvNc0.pParams->pAdvData = (uint8_t*)aBeaconData;
          //RF_cmdAdvNc0.pParams->advLen = ADVLEN;
          //increment aBeacon counter
          advCount++;
          //set time to next advertisement
          time += PACKET_INTERVAL;
        }
        else if (advCount < ABEACON_COUNT) // aBeacon packet
        {
          //set time for next packet
          time += PACKET_INTERVAL;
          //increment aBeacon counter
          advCount++;
        }
        else                              //bBeacon packet
        {
          //reset aBeacon counter
          advCount = 1;
          //don't advertise again until end of period
          time += ADV_PERIOD;
        }

        /* Set absolute TX time to utilize automatic power management */
        RF_cmdAdvNc0.startTime = time;

        /* Send packet */
        RF_EventMask result = RF_runCmd(rfHandle, (RF_Op*)&RF_cmdAdvNc0, RF_PriorityNormal, NULL, 0);
        if (!(result & RF_EventLastCmdDone))
        {
          /* Error */
          while(1);
        }

#ifndef POWER_MEASUREMENT
        PIN_setOutputValue(ledPinHandle, Board_PIN_LED1,!PIN_getOutputValue(Board_PIN_LED1));
#endif
    }
}

