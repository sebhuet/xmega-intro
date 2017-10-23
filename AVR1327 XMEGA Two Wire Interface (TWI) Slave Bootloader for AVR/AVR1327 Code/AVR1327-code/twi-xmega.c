/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file ********************************************************************
*
* Atmel Corporation
*
* File              : twi-xmega.c
* Compiler          : IAR Embedded Workbench EWAVR C Compiler
* Revision          : $5.51
* Date              : $Date: 23 Aug 11
* Updated by        : $Author: Marv Kausch
*
* Support mail      : avr@atmel.com
*
* Supported devices : The example is written for virtually all xmega devices that have one or more TWI peripherals
*
* AppNote           :avr1327-twi_bootloader-xmega
*
* Description       : TWI driver code C file for AVR1327
*
*
*
****************************************************************************/

/*! \page License
Copyright (c) 2011 Atmel Corporation. All rights reserved. 
 
Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer. 

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

3. The name of Atmel may not be used to endorse or promote products derived 
from this software without specific prior written permission.  

4. This software may only be redistributed and used in connection with an Atmel 
AVR product. 

THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED 
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE 
EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */


/* Doxygen documentation mainpage ********************************************/
/*! \mainpage
 * \section intro Introduction
 * This documents the software for application note AVR1327.
 *
 * \section compinfo Compilation Info
 * This software was compiled with IAR C/C++ Compiler for AVR
 * the source have to be modified.\n
 *
 * \section deviceinfo Device Info
 * Virtually all XMEGA AVRs that contain one or more TWI peripherals
 *
 * \section codesize Code Size
 * 1382 Bytes if optimized for size High Optimization level, 352 Bytes SRAM bytes, one EEPROM byte
 *
 *
 * \section contactinfo Contact Info
 * For more info about Atmel AVR visit http://www.atmel.com/products/AVR/ \n
 * For application notes visit 
 * http://www.atmel.com/dyn/products \n
 * Support mail: avr@atmel.com
 */



#include <stdint.h>
#include <stdbool.h>

#include <inavr.h>
#include <ioavr.h>

#include "conf-twi.h"
#include "twi.h"


static uint16_t reg_address;
static uint8_t wr_i;
static bool data_sent;


void twi_init(void)
{
        TWI_BASE.CTRLA = TWI_SLAVE_ENABLE_bm;
	TWI_BASE.ADDR = TWI_ADDRESS << 1;
}


uint8_t twi_poll_bus(void)
{
        uint8_t status = TWI_BASE.STATUS;

        if ((status & (TWI_SLAVE_DIF_bm | TWI_SLAVE_APIF_bm | TWI_SLAVE_BUSERR_bm)) == 0x00) {
                return TWI_STATE_IDLE;
        }
 
        uint8_t retval = TWI_STATE_IDLE;
	if ((status & (TWI_SLAVE_DIF_bm | TWI_SLAVE_DIR_bm)) == TWI_SLAVE_DIF_bm) {
                if (wr_i == 0) {
                        ++wr_i;    
                        reg_address = TWI_BASE.DATA;
                } else if (wr_i == 1) {
                        ++wr_i;
                        /* Write data to register. */
                        reg_address |= (((uint16_t)TWI_BASE.DATA) << 8);
                } else {
                        twi_write_reg(reg_address, TWI_BASE.DATA);
                        ++reg_address;
                }
                        
                /* ACK */
                TWI_BASE.CTRLB = TWI_SLAVE_CMD_RESPONSE_gc;
                TWI_BASE.STATUS |= TWI_SLAVE_DIF_bm;
        } else if ((status & (TWI_SLAVE_DIF_bm | TWI_SLAVE_DIR_bm)) == (TWI_SLAVE_DIF_bm | TWI_SLAVE_DIR_bm)) {
                if (((status & TWI_SLAVE_RXACK_bm) == TWI_SLAVE_RXACK_bm) &&\
                                (data_sent == true)) {
                        /* Nack received - complete transaction. */
                         TWI_BASE.CTRLB = TWI_SLAVE_CMD_COMPTRANS_gc;
                } else {
                        data_sent = true;
                        TWI_BASE.DATA = twi_read_reg(reg_address);
                        ++reg_address;

                        TWI_BASE.CTRLB = TWI_SLAVE_CMD_RESPONSE_gc;
                }
                
                TWI_BASE.STATUS |= TWI_SLAVE_DIF_bm;
        } else if ((status & (TWI_SLAVE_APIF_bm | TWI_SLAVE_AP_bm)) == TWI_SLAVE_APIF_bm) {
                /* Stop condition */
                TWI_BASE.STATUS |= TWI_SLAVE_APIF_bm;
                retval = TWI_STATE_STOP;
        } else if ((status & (TWI_SLAVE_APIF_bm | TWI_SLAVE_AP_bm)) == (TWI_SLAVE_APIF_bm | TWI_SLAVE_AP_bm)) {
                /*Address match - send ack */
                data_sent = false;
                wr_i = 0;
                
                TWI_BASE.CTRLB = TWI_SLAVE_CMD_RESPONSE_gc;
                TWI_BASE.STATUS |= TWI_SLAVE_APIF_bm;
        } else {
                /* Unknown state or error. */
                if ((status & (TWI_SLAVE_BUSERR_bm | TWI_SLAVE_COLL_bm)) != 0x00) {
                        /* Error or collision... */
                        retval = TWI_STATE_ERROR;
                }
        }
                
        if (reg_address == TWI_LAST_REG) {
	        reg_address = 0;
	}
        
        return retval;
}