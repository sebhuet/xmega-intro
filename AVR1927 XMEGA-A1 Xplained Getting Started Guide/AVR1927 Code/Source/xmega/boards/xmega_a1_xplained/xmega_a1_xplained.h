/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief XMEGA-A1-XPLAINED board header file.
 *
 * This file contains definitions and services related to the features of the
 * XMEGA-A1-XPLAINED board.
 *
 * To use this board, define BOARD=XMEGA_A1_XPLAINED.
 *
 * - Compiler:           IAR EWAVR and GNU GCC for AVR
 * - Supported devices:  ATxmegaA1 devices
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/**
 * Copyright (C) 2010 Atmel Corporation. All rights reserved.
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 * Atmel AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */
#ifndef _XMEGA_A1_XPLAINED_H_
#define _XMEGA_A1_XPLAINED_H_

#include <compiler.h>

//! \name Miscellaneous data
//@{
//@}

//! Number of LEDs.
#define LED_COUNT                        8

//! \name GPIO Connections of LEDs
//@{
#define LED0_GPIO                        IOPORT_CREATE_PIN(PORTE, 0)
#define LED1_GPIO                        IOPORT_CREATE_PIN(PORTE, 1)
#define LED2_GPIO                        IOPORT_CREATE_PIN(PORTE, 2)
#define LED3_GPIO                        IOPORT_CREATE_PIN(PORTE, 3)
#define LED4_GPIO                        IOPORT_CREATE_PIN(PORTE, 4)
#define LED5_GPIO                        IOPORT_CREATE_PIN(PORTE, 5)
#define LED6_GPIO                        IOPORT_CREATE_PIN(PORTE, 6)
#define LED7_GPIO                        IOPORT_CREATE_PIN(PORTE, 7)
//@}

//! \name GPIO Connections of Push Buttons
//@{
#define GPIO_PUSH_BUTTON_0               IOPORT_CREATE_PIN(PORTD, 0)
#define GPIO_PUSH_BUTTON_1               IOPORT_CREATE_PIN(PORTD, 1)
#define GPIO_PUSH_BUTTON_2               IOPORT_CREATE_PIN(PORTD, 2)
#define GPIO_PUSH_BUTTON_3               IOPORT_CREATE_PIN(PORTD, 3)
#define GPIO_PUSH_BUTTON_4               IOPORT_CREATE_PIN(PORTD, 4)
#define GPIO_PUSH_BUTTON_5               IOPORT_CREATE_PIN(PORTD, 5)
// NB! PORTR pin 0
#define GPIO_PUSH_BUTTON_6               IOPORT_CREATE_PIN(PORTR, 0)
// NB! PORTR pin 1
#define GPIO_PUSH_BUTTON_7               IOPORT_CREATE_PIN(PORTR, 1)
//@}

//! \name ADC Connection of the Light Sensor
//@{
#define LIGHT_SENSOR_ADC_MODULE          ADCB
#define LIGHT_SENSOR_ADC_INPUT           ADCCH_POS_PIN1
#define LIGHT_SENSOR_SIGNAL_PIN          IOPORT_CREATE_PIN(PORTB, 1)
//@}

//! \name ADC Connection of the Temperature Sensor
//@{
#define TEMPERATURE_SENSOR_ADC_MODULE    ADCB
#define TEMPERATURE_SENSOR_ADC_INPUT     ADCCH_POS_PIN0
#define TEMPERATURE_SENSOR_SIGNAL_PIN    IOPORT_CREATE_PIN(PORTB, 0)
#define TEMPERATURE_SENSOR_ENABLE_PIN    IOPORT_CREATE_PIN(PORTB, 3)
#define TEMPERATURE_SENSOR_ENABLE_LEVEL  false
//@}

//! \name DAC connection and control of the Speaker
//@{
#define SPEAKER_DAC_MODULE               DACB
#define SPEAKER_DAC_CHANNEL              DAC_CH0
#define SPEAKER_SIGNAL_PIN               IOPORT_CREATE_PIN(PORTB, 2)
#define SPEAKER_ENABLE_PIN               IOPORT_CREATE_PIN(PORTQ, 3)
#define SPEAKER_ENABLE_LEVEL             true
//@}

//! \name External oscillator
//@{
#define BOARD_XOSC_HZ                    8000000
#define BOARD_XOSC_TYPE                  XOSC_TYPE_XTAL
#define BOARD_XOSC_STARTUP_US            2000
//@}

#endif  // _XMEGA_A1_XPLAINED_H_
