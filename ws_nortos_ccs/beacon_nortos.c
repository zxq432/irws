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
#include <irws_ex_include.h>

/* Driverlib Header files */
#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)

/* Board Header files */
#include "Board.h"
#include "smartrf_settings/smartrf_settings.h"

#if(sensor==23)
#include "TSOP38140sce/scif_tsop38140.h"
#elif(sensor==711)
#include "HX711sce/scif_hx711.h"
#endif

/***** Defines *****/

/* TX Configuration */
#define PACKET_INTERVAL                 50*4000   //50ms @ 4 MHz
#define ABEACON_COUNT                   4
//#define ADV_PERIOD                      (6000 * 4000) - ((PACKET_INTERVAL * ABEACON_COUNT) + BBEACON_DELAY)
#define ADVLEN                          3
#define ADCNexp    2
//#define ADCN       (1 << ADCNexp)

#if(sensor==23)
#define IR_PEROID               6
#elif(sensor==711)
#define IR_PEROID               120
#endif
#define DATA_HOURS              360 / IR_PEROID - 1
#define BATT_HOURS              4 * 3600 / IR_PEROID
#define CCOUNT                  2
#define ADC_BATT2_6V            86

/* Do power measurement */
//#define POWER_MEASUREMENT

/***** Prototypes *****/
extern void Power_idleFunc(void);
extern uint32_t scifOsalEnterCriticalSection(void);
extern void scifOsalLeaveCriticalSection(uint32_t key);
#if(sensor==23)
extern void tsop38140Start();
#elif(sensor==711)
extern void hx711Start();
#endif
//extern void hx711adcAvg(int16_t *p);
//extern void hx711adcN();

/***** Variable declarations *****/
volatile bool dataReceived = false;

static RF_Object rfObject;
static RF_Handle rfHandle;

/* Pin driver handle */
//static PIN_Handle ledPinHandle;
//static PIN_State ledPinState;

//uint8_t adcNexp = ADCNexp;
uint8_t adcN = 4; //1 << ADCNexp;
uint8_t advLen = ADVLEN;
static char beaconData[ADVLEN+2] = {0x00, 0x00, 0x00, 0x00, 0x00};

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
    uint8_t n;
    uint32_t key;
    uint32_t batt;
//    uint32_t time;
    RF_Params rfParams;
    RF_Params_init(&rfParams);

    /* Open LED pins */
    /*ledPinHandle = PIN_open(&ledPinState, pinTable);
    if (ledPinHandle == NULL)
    {
        while(1);
    }
    sleep(1);
    */
#ifdef POWER_MEASUREMENT
    /* Shutdown external flash */
    Board_shutDownExtFlash();
#if !defined(__CC1352R1_LAUNCHXL_BOARD_H__) && !defined(__CC26X2R1_LAUNCHXL_BOARD_H__)
    /* Route out PA active pin to Board_DIO30_SWPWR */
    PINCC26XX_setMux(ledPinHandle, Board_DIO30_SWPWR, PINCC26XX_MUX_RFC_GPO1);
#endif
#endif

#if(sensor==23)
    ADV_DATA.SQN.TYPE = 0;
    tsop38140Start();
#elif(sensor==711)
    ADV_DATA.SQN.TYPE = 1;
    hx711Start();
#endif

    /* Request access to the radio */
    rfHandle = RF_open(&rfObject, &RF_ble, (RF_RadioSetup*)&RF_cmdBleRadioSetup, &rfParams);

    while(1)
    {
        key = scifOsalEnterCriticalSection();
        while (dataReceived == false)
        {
           Power_idleFunc();
           scifOsalLeaveCriticalSection(key);
           // Let 'readCallback()' execute...
           key = scifOsalEnterCriticalSection();
        }
        dataReceived = false;
        scifOsalLeaveCriticalSection(key);

#if(sensor==23)
        ADV_DATA.SQN.BUZZ0 = scifTsop38140TaskData.irled.output.buzz;
        ADV_DATA.SQN.BUZZ1 = scifTsop38140TaskData.irled.state.buzz;

        if(scifTsop38140TaskData.irled.state.bCount >= scifTsop38140TaskData.irled.cfg.bCount) {
            scifTsop38140TaskData.irled.state.bCount = 0;
#elif(sensor==711)
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

#if(sensor==23)
        beaconData[2] = (batt >> 3) - 0x40;
        advLen = ADVLEN;
        } else
            advLen = ADVLEN - 1;
#elif(sensor==711)
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
        for (n = 0;  n < adcN; n++) // aBeacon packet
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
#if(sensor==23)
        scifTsop38140TaskData.irled.state.exit = 0;
#elif(sensor==711)
        //hx711adcN();
        scifHx711TaskData.adc24bit.state.exit = 0;
#endif
#ifndef POWER_MEASUREMENT
        //PIN_setOutputValue(ledPinHandle, Board_PIN_LED1,!PIN_getOutputValue(Board_PIN_LED1));
#endif
    } // while(1)
}  // mainThread
