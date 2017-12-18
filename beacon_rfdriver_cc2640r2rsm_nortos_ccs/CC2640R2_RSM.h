/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       CC2640R2_RSM.h
 *
 *  @brief      CC2640R2 RSM Board Specific header file.
 *
 *  The CC2640R2_RSM header file should be included in an application as
 *  follows:
 *  @code
 *  #include "CC2640R2_RSM.h"
 *  @endcode
 *
 *  ============================================================================
 */
#ifndef __CC2640R2_RSM_BOARD_H__
#define __CC2640R2_RSM_BOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <ti/drivers/PIN.h>
#include <ti/devices/cc26x0r2/driverlib/ioc.h>

/* Externs */
extern const PIN_Config BoardGpioInitTable[];

/* Defines */
#define CC2640R2_RSM

/* Mapping of pins to board signals using general board aliases
 *      <board signal alias>        <pin mapping>   <comments>
 */

/* Analog Capable DIOs */
#define CC2640R2_RSM_DIO5_ANALOG          IOID_5
#define CC2640R2_RSM_DIO6_ANALOG          IOID_6
#define CC2640R2_RSM_DIO7_ANALOG          IOID_7
#define CC2640R2_RSM_DIO8_ANALOG          IOID_8
#define CC2640R2_RSM_DIO9_ANALOG          IOID_9

/* Digital IOs */
#define CC2640R2_RSM_DIO0                  IOID_0
// #define CC2640R2_RSM_DIO1_RFSW             IOID_1
// #define CC2640R2_RSM_DIO1                  IOID_1
// #define CC2640R2_RSM_DIO2                 IOID_2
// #define CC2640R2_RSM_DIO3                 IOID_3
#define CC2640R2_RSM_DIO3_TDO             IOID_3
#define CC2640R2_RSM_DIO4_TDI             IOID_4
// #define CC2640R2_RSM_DIO30_SWPWR           IOID_30
#define CC2640R2_RSM_DIO5                 IOID_5
#define CC2640R2_RSM_DIO6                 IOID_6
#define CC2640R2_RSM_DIO7                 IOID_7
#define CC2640R2_RSM_DIO8                 IOID_8
#define CC2640R2_RSM_DIO9                 IOID_9

// /* Discrete Inputs */
#define CC2640R2_RSM_PIN_BTN1              PIN_UNASSIGNED
#define CC2640R2_RSM_PIN_BTN2              PIN_UNASSIGNED


 /* GPIO */
#define CC2640R2_RSM_GPIO_LED_ON           1
#define CC2640R2_RSM_GPIO_LED_OFF          0
// 
/* I2C */
#define CC2640R2_RSM_I2C0_SCL0             PIN_UNASSIGNED
#define CC2640R2_RSM_I2C0_SDA0             PIN_UNASSIGNED

/* LCD (430BOOST - Sharp96 Rev 1.1) */
// #define CC2640R2_RSM_LCD_CS                IOID_24  /* SPI chip select */
// #define CC2640R2_RSM_LCD_EXTCOMIN          IOID_12  /* External COM inversion */
// #define CC2640R2_RSM_LCD_ENABLE            IOID_22  /* LCD enable */
// #define CC2640R2_RSM_LCD_POWER             IOID_23  /* LCD power control */
// #define CC2640R2_RSM_LCD_CS_ON             1
// #define CC2640R2_RSM_LCD_CS_OFF            0

/* LEDs */
#define CC2640R2_RSM_PIN_LED_ON            1
#define CC2640R2_RSM_PIN_LED_OFF           0
#define CC2640R2_RSM_PIN_RLED              PIN_UNASSIGNED
#define CC2640R2_RSM_PIN_GLED              PIN_UNASSIGNED

/* PWM Outputs */
#define CC2640R2_RSM_PWMPIN0               CC2640R2_RSM_PIN_RLED
#define CC2640R2_RSM_PWMPIN1               CC2640R2_RSM_PIN_GLED
#define CC2640R2_RSM_PWMPIN2               PIN_UNASSIGNED
#define CC2640R2_RSM_PWMPIN3               PIN_UNASSIGNED
#define CC2640R2_RSM_PWMPIN4               PIN_UNASSIGNED
#define CC2640R2_RSM_PWMPIN5               PIN_UNASSIGNED
#define CC2640R2_RSM_PWMPIN6               PIN_UNASSIGNED
#define CC2640R2_RSM_PWMPIN7               PIN_UNASSIGNED

/* SPI */
#define CC2640R2_RSM_SPI_FLASH_CS          PIN_UNASSIGNED
#define CC2640R2_RSM_FLASH_CS_ON           0
#define CC2640R2_RSM_FLASH_CS_OFF          1

/* SPI Board */
#define CC2640R2_RSM_SPI0_MISO             PIN_UNASSIGNED          /* RF1.20 */
#define CC2640R2_RSM_SPI0_MOSI             PIN_UNASSIGNED          /* RF1.18 */
#define CC2640R2_RSM_SPI0_CLK              PIN_UNASSIGNED         /* RF1.16 */
#define CC2640R2_RSM_SPI0_CSN              PIN_UNASSIGNED
#define CC2640R2_RSM_SPI1_MISO             PIN_UNASSIGNED
#define CC2640R2_RSM_SPI1_MOSI             PIN_UNASSIGNED
#define CC2640R2_RSM_SPI1_CLK              PIN_UNASSIGNED
#define CC2640R2_RSM_SPI1_CSN              PIN_UNASSIGNED

/* UART Board */
#define CC2640R2_RSM_UART_RX               IOID_1          /* RXD */
#define CC2640R2_RSM_UART_TX               IOID_2          /* TXD */
#define CC2640R2_RSM_UART_CTS              PIN_UNASSIGNED         /* CTS */
#define CC2640R2_RSM_UART_RTS              PIN_UNASSIGNED         /* RTS */

/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 */
void CC2640R2_RSM_initGeneral(void);

/*!
 *  @brief  Turn off the external flash on LaunchPads
 *
 */
void CC2640R2_RSM_shutDownExtFlash(void);

/*!
 *  @def    CC2640R2_RSM_ADCBufName
 *  @brief  Enum of ADCBufs
 */
typedef enum CC2640R2_RSM_ADCBufName {
    CC2640R2_RSM_ADCBUF0 = 0,

    CC2640R2_RSM_ADCBUFCOUNT
} CC2640R2_RSM_ADCBufName;

/*!
 *  @def    CC2640R2_RSM_ADCBuf0SourceName
 *  @brief  Enum of ADCBuf channels
 */
typedef enum CC2640R2_RSM_ADCBuf0ChannelName {
    CC2640R2_RSM_ADCBUF0CHANNEL0 = 0,
    CC2640R2_RSM_ADCBUF0CHANNEL1,
    CC2640R2_RSM_ADCBUF0CHANNEL2,
    CC2640R2_RSM_ADCBUF0CHANNEL3,
    CC2640R2_RSM_ADCBUF0CHANNEL4,
    CC2640R2_RSM_ADCBUF0CHANNEL5,
    CC2640R2_RSM_ADCBUF0CHANNEL6,
    CC2640R2_RSM_ADCBUF0CHANNEL7,
    CC2640R2_RSM_ADCBUF0CHANNELVDDS,
    CC2640R2_RSM_ADCBUF0CHANNELDCOUPL,
    CC2640R2_RSM_ADCBUF0CHANNELVSS,

    CC2640R2_RSM_ADCBUF0CHANNELCOUNT
} CC2640R2_RSM_ADCBuf0ChannelName;

/*!
 *  @def    CC2640R2_RSM_ADCName
 *  @brief  Enum of ADCs
 */
typedef enum CC2640R2_RSM_ADCName {
    CC2640R2_RSM_ADC0 = 0,
    CC2640R2_RSM_ADC1,
    CC2640R2_RSM_ADC2,
    CC2640R2_RSM_ADC3,
    CC2640R2_RSM_ADC4,
    CC2640R2_RSM_ADC5,
    CC2640R2_RSM_ADC6,
    CC2640R2_RSM_ADC7,
    CC2640R2_RSM_ADCDCOUPL,
    CC2640R2_RSM_ADCVSS,
    CC2640R2_RSM_ADCVDDS,

    CC2640R2_RSM_ADCCOUNT
} CC2640R2_RSM_ADCName;

/*!
 *  @def    CC2640R2_RSM_CryptoName
 *  @brief  Enum of Crypto names
 */
typedef enum CC2640R2_RSM_CryptoName {
    CC2640R2_RSM_CRYPTO0 = 0,

    CC2640R2_RSM_CRYPTOCOUNT
} CC2640R2_RSM_CryptoName;

/*!
 *  @def    CC2640R2_RSM_GPIOName
 *  @brief  Enum of GPIO names
 */
typedef enum CC2640R2_RSM_GPIOName {
    CC2640R2_RSM_GPIO_S1 = 0,
    CC2640R2_RSM_GPIO_S2,
    CC2640R2_RSM_GPIO_LED_GREEN,
    CC2640R2_RSM_GPIO_LED_RED,
    CC2640R2_RSM_GPIO_SPI_FLASH_CS,

    CC2640R2_RSM_GPIOCOUNT
} CC2640R2_RSM_GPIOName;

/*!
 *  @def    CC2640R2_RSM_GPTimerName
 *  @brief  Enum of GPTimer parts
 */
typedef enum CC2640R2_RSM_GPTimerName {
    CC2640R2_RSM_GPTIMER0A = 0,
    CC2640R2_RSM_GPTIMER0B,
    CC2640R2_RSM_GPTIMER1A,
    CC2640R2_RSM_GPTIMER1B,
    CC2640R2_RSM_GPTIMER2A,
    CC2640R2_RSM_GPTIMER2B,
    CC2640R2_RSM_GPTIMER3A,
    CC2640R2_RSM_GPTIMER3B,

    CC2640R2_RSM_GPTIMERPARTSCOUNT
} CC2640R2_RSM_GPTimerName;

/*!
 *  @def    CC2640R2_RSM_GPTimers
 *  @brief  Enum of GPTimers
 */
typedef enum CC2640R2_RSM_GPTimers {
    CC2640R2_RSM_GPTIMER0 = 0,
    CC2640R2_RSM_GPTIMER1,
    CC2640R2_RSM_GPTIMER2,
    CC2640R2_RSM_GPTIMER3,

    CC2640R2_RSM_GPTIMERCOUNT
} CC2640R2_RSM_GPTimers;

/*!
 *  @def    CC2640R2_RSM_I2CName
 *  @brief  Enum of I2C names
 */
typedef enum CC2640R2_RSM_I2CName {
    CC2640R2_RSM_I2C0 = 0,

    CC2640R2_RSM_I2CCOUNT
} CC2640R2_RSM_I2CName;

/*!
 *  @def    CC2640R2_RSM_NVSName
 *  @brief  Enum of NVS names
 */
typedef enum CC2640R2_RSM_NVSName {
    CC2640R2_RSM_NVSCC26XX0 = 0,
    CC2640R2_RSM_NVSSPI25X0,

    CC2640R2_RSM_NVSCOUNT
} CC2640R2_RSM_NVSName;

/*!
 *  @def    CC2640R2_RSM_PWM
 *  @brief  Enum of PWM outputs
 */
typedef enum CC2640R2_RSM_PWMName {
    CC2640R2_RSM_PWM0 = 0,
    CC2640R2_RSM_PWM1,
    CC2640R2_RSM_PWM2,
    CC2640R2_RSM_PWM3,
    CC2640R2_RSM_PWM4,
    CC2640R2_RSM_PWM5,
    CC2640R2_RSM_PWM6,
    CC2640R2_RSM_PWM7,

    CC2640R2_RSM_PWMCOUNT
} CC2640R2_RSM_PWMName;

/*!
 *  @def    CC2640R2_RSM_SPIName
 *  @brief  Enum of SPI names
 */
typedef enum CC2640R2_RSM_SPIName {
    CC2640R2_RSM_SPI0 = 0,
    CC2640R2_RSM_SPI1,

    CC2640R2_RSM_SPICOUNT
} CC2640R2_RSM_SPIName;

/*!
 *  @def    CC2640R2_RSM_UARTName
 *  @brief  Enum of UARTs
 */
typedef enum CC2640R2_RSM_UARTName {
    CC2640R2_RSM_UART0 = 0,

    CC2640R2_RSM_UARTCOUNT
} CC2640R2_RSM_UARTName;

/*!
 *  @def    CC2640R2_RSM_UDMAName
 *  @brief  Enum of DMA buffers
 */
typedef enum CC2640R2_RSM_UDMAName {
    CC2640R2_RSM_UDMA0 = 0,

    CC2640R2_RSM_UDMACOUNT
} CC2640R2_RSM_UDMAName;

/*!
 *  @def    CC2640R2_RSM_WatchdogName
 *  @brief  Enum of Watchdogs
 */
typedef enum CC2640R2_RSM_WatchdogName {
    CC2640R2_RSM_WATCHDOG0 = 0,

    CC2640R2_RSM_WATCHDOGCOUNT
} CC2640R2_RSM_WatchdogName;

#ifdef __cplusplus
}
#endif

#endif /* __CC2640R2_RSM_BOARD_H__ */
