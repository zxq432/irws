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
//#include <unistd.h>
/* Standard C Libraries */
#include <stdlib.h>

// DriverLib
#include <driverlib/aon_batmon.h>

#include "irws_tirtos.h"

#include "ex_include_tirtos.h"
#if(sensor==23)
#include "TSOP38140sce/scif_tsop38140.h"
#elif(sensor==711)
#include "HX711sce/scif_hx711.h"
#endif

// What is the advertising interval when device is discoverable (units of 625us, 160=100ms)
#define DEFAULT_ADVERTISING_INTERVAL          160

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

uint8_t adcN = 4;
uint8_t advLen = ADVLEN;
adv_data_t ADV_DATA;

// Pointer to application callback
scifAlertCB_t appScifAlertHandler = NULL;

void sensor2adv(void);

void scCtrlReadyCallback(void) {

} // scCtrlReadyCallback


void scTaskAlertCallback(void) {

    sensor2adv();

} // scTaskAlertCallback



/*
PIN_Config pLedPinTable[] = {
    Board_GLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_RLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};
PIN_State ledPinState;
*/

void Scif_start(scifAlertCB_t appScifCB) {
    // Initialize the Sensor Controller
    scifOsalInit();
    scifOsalRegisterCtrlReadyCallback(scCtrlReadyCallback);
    scifOsalRegisterTaskAlertCallback(scTaskAlertCallback);
#if(sensor==23)
    scifInit(&scifTsop38140DriverSetup);
    IOCIODrvStrengthSet(IOID_9, IOC_CURRENT_4MA, PIN_DRVSTR_MAX);
    IOCIODrvStrengthSet(IOID_0, IOC_CURRENT_8MA, PIN_DRVSTR_MAX);
    IOCIODrvStrengthSet(IOID_1, IOC_CURRENT_8MA, PIN_DRVSTR_MAX);
    scifTsop38140StartRtcTicksNow(0x00060000);  //6s

    ADV_DATA.SQN.TYPE = 0;
    scifStartTasksNbl(BV(SCIF_TSOP38140_IRLED_TASK_ID));
#elif(sensor==711)
    scifInit(&scifHx711DriverSetup);
    IOCIODrvStrengthSet(IOID_9, IOC_CURRENT_4MA, PIN_DRVSTR_MAX);
    scifHx711StartRtcTicksNow(0x00400000);  //64s

    ADV_DATA.SQN.TYPE = 1;
    scifStartTasksNbl(1<<SCIF_HX711_ADC24BIT_TASK_ID);
#endif

    appScifAlertHandler = appScifCB;
}

void sensor2adv(void) {
    uint32_t batt;
        // Clear the ALERT interrupt source
        scifClearAlertIntSource();

#if(sensor==23)
        ADV_DATA.SQN.BUZZ0 = scifTsop38140TaskData.irled.output.buzz;
        ADV_DATA.SQN.BUZZ1 = scifTsop38140TaskData.irled.state.buzz;

        if(scifTsop38140TaskData.irled.state.bCount >= scifTsop38140TaskData.irled.cfg.bCount) {
            scifTsop38140TaskData.irled.state.bCount = 0;
#elif(sensor==711)
            ADV_DATA.SQN.BUZZ0 = scifHx711TaskData.adc24bit.output.buzz;
            ADV_DATA.SQN.BUZZ1 = scifHx711TaskData.adc24bit.state.buzz;

            //hx711adcAvg(&ADV_DATA.ADC);
            *((uint16_t *) ADV_DATA.ADC) = scifHx711TaskData.adc24bit.output.adcVal;

        if(scifHx711TaskData.adc24bit.state.bCount >= scifHx711TaskData.adc24bit.cfg.bCount) {
            scifHx711TaskData.adc24bit.state.bCount = 0;
#endif
        // Battery voltage (bit 10:8 - integer, but 7:0 fraction)
        batt = AONBatMonBatteryVoltageGet();
        //batt = (batt * 125) >> 5; // convert V to mV

#if(sensor==23)
        ADV_DATA.BATT = (batt >> 3) - 0x40;
        ADV_DATA.LEN = ADVLEN - 1;
        } else
            ADV_DATA.LEN = ADVLEN - 2;
#elif(sensor==711)
        ADV_DATA.BATT = (batt >> 3) - 0x40;
        ADV_DATA.LEN = ADVLEN + 1;
        } else
            ADV_DATA.LEN = ADVLEN;
#endif
          //set adv data
        if (appScifAlertHandler != NULL)
        {
          // Notify the application
          (*appScifAlertHandler)(&ADV_DATA);
        }

          //ADV_DATA.SQN.RND++;
        ADV_DATA.SQN.SEQ++;
#if(sensor==23)
        scifTsop38140TaskData.irled.state.exit = 0;
#elif(sensor==711)
        scifHx711TaskData.adc24bit.state.exit = 0;
#endif
        // Acknowledge the alert event
        scifAckAlertEvents();
} // taskFxn

/*********************************************************************
*********************************************************************/
