/*
 * HX711.c
 *
 *  Created on: 2017��12��2��
 *      Author: Administrator
 */
#include <irws_ex_include.h>
#include "TSOP38140sce/scif_tsop38140.h"

#include <ti/drivers/PIN.h>
//#define ADC_SAMPLE_BUFFER_SIZE (SCIF_HX711_ADC24BIT_BUFFER_SIZE >> 1);

extern bool dataReceived;

void ctrlReadyCallback(void) {

} // ctrlReadyCallback




void taskAlertCallback(void) {

    // Clear the ALERT interrupt source
    scifClearAlertIntSource();

    /* Notify the main thread that data has been received */
    dataReceived = true;

    // Acknowledge the alert event
    scifAckAlertEvents();

} // taskAlertCallback

void tsop38140Start() {

    // Enable power domains
    PRCMPowerDomainOn(PRCM_DOMAIN_PERIPH);
    PRCMLoadSet();
    while (PRCMPowerDomainStatus(PRCM_DOMAIN_PERIPH) != PRCM_DOMAIN_POWER_ON);

    // Enable the GPIO module
    PRCMPeripheralRunEnable(PRCM_PERIPH_GPIO);
    PRCMPeripheralSleepEnable(PRCM_PERIPH_GPIO);
    PRCMPeripheralDeepSleepEnable(PRCM_PERIPH_GPIO);
    PRCMLoadSet();
    while (!PRCMLoadGet());

    // In this example, we keep the AUX domain access permanently enabled
    //scifOsalEnableAuxDomainAccess();

    // Initialize and start the Sensor Controller
    scifOsalRegisterCtrlReadyCallback(ctrlReadyCallback);
    scifOsalRegisterTaskAlertCallback(taskAlertCallback);
    scifInit(&scifTsop38140DriverSetup);
    IOCIODrvStrengthSet(IOID_9, IOC_CURRENT_4MA, PIN_DRVSTR_MAX);
    IOCIODrvStrengthSet(IOID_0, IOC_CURRENT_8MA, PIN_DRVSTR_MAX);
    scifTsop38140StartRtcTicksNow(0x00060000);  //6s
    IntMasterEnable();
    AONRTCEnable();

    // Configure ADC samples need to be collected
    //scifHx711TaskData.adc24bit.cfg.adcN = n;
    scifStartTasksNbl(1<<SCIF_TSOP38140_IRLED_TASK_ID);

    // Main loop
    //while (1) {
    //}

} // hx711adcStart

/*
void hx711adcAvg(int16_t* p) {
    uint32_t sum = 0;
    uint8_t i;

    for(i = 0; i != adcN; i++) sum += scifHx711TaskData.adc24bit.output.pSamples[i];
    *p = (sum >> adcNexp);
}

void hx711adcN() {
    scifHx711TaskData.adc24bit.state.adcN = 0;
    scifHx711TaskData.adc24bit.state.exit = 0;
}
*/
