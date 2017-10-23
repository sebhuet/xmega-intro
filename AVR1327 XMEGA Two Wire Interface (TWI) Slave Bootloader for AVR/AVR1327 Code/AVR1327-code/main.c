/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file ********************************************************************
*
* Atmel Corporation
*
* File              : avr1617_xmega_freq_cntr.c
* Compiler          : IAR Embedded Workbench EWAVR C Compiler
* Revision          : $5.51
* Date              : $Date: 23 Aug 11
* Updated by        : $Author: Marv Kausch
*
* Support mail      : avr@atmel.com
*
* Supported devices : The example is written for virtually all xmega devices using 2-3 Event SYstem channels and 2-3 Timer/counters
*
* AppNote           : AVR1617 - Frequency Measurement Using the XMEGA Event System
*
* Description       : Application code C file for AVR1617
*
*
*
****************************************************************************/


/*! \page License
Copyright (c) 2009 Atmel Corporation. All rights reserved. 
 
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
 * This documents the software for application note AVR1617.
 *
 * \section compinfo Compilation Info
 * This software was compiled with WinAVR GCC 20100110.
 * the source have to be modified.\n
 *
 * \section deviceinfo Device Info
 * Virtually all XMEGA AVRs that offer 32 MHz clock operation, 2-3 16 bit Timer/Counters and 2-3 Event SYstem channels
 *
 * \section codesize Code Size
 * 1082 program bytes Optimization level -O1), 7 SRAM bytes
 *
 *
 * \section contactinfo Contact Info
 * For more info about Atmel AVR visit http://www.atmel.com/products/AVR/ \n
 * For application notes visit 
 * http://www.atmel.com/dyn/products/app_notes.asp?family_id=607 \n
 * Support mail: avr@atmel.com
 */



#include <stdint.h>
#include <stdbool.h>

#include <ioavr.h>
#include <inavr.h>

#include "ccp.h"
#include "sp_driver.h"
#include "twi.h"


#if defined(__ATxmega16D4__)
        #define APP_PAGE_SIZE          (256u)
        #define APP_PAGE_CNT           (64u)
        #define ADDRESS_SHIFT          (8)
#elif defined(__ATxmega32A4__) || defined(__ATxmega32D4__)
        #define APP_PAGE_SIZE          (256u)
        #define APP_PAGE_CNT           (128u)
        #define ADDRESS_SHIFT          (8)
#else
#error 'Device not supported yet'
#endif


#define APPLICATION_SIZE        (APP_PAGE_SIZE * APP_PAGE_CNT)
#define BOOTLOADER_WILDCARD     (0xAA)
#define WILDCARD_ADDRESS        (0x00)

#define BOOT_PORT               (PORTB)
#define BOOT_PIN_MASK           (PIN3_bm)


enum bootloader_cmds {
        CMD_NONE                = 0x00,
        CMD_RESET               = 0x01,
        CMD_APP_EXEC            = 0x02,
        CMD_ERASE_APP           = 0x03,
        CMD_PROGRAM_FLASH       = 0x04,
        CMD_PROGRAM_EEPROM      = 0x05,
        CMD_FLASH_CRC           = 0x06,
        CMD_EEPROM_CRC          = 0x07        
};


struct bootloader_memory_map {
        uint16_t page_size;
        uint16_t page_count;
        uint16_t id;
        uint16_t address;
        uint16_t page_bytes;
        uint8_t page[APP_PAGE_SIZE];
        uint8_t cmd;
        uint16_t crc;
};


union bootloader_memory_access {
        struct bootloader_memory_map structured;
        uint8_t direct[sizeof(struct bootloader_memory_map)];
};


static uint8_t temp_b;
static uint8_t *temp_ptr;
static uint16_t temp_w; 
static uint16_t crc;
static uint32_t page_address;
static uint32_t temp_dw;
static union bootloader_memory_access bootmemory;


static uint16_t crc_ccitt_update(uint16_t crc, uint8_t data)
{
    data ^= crc & 0xFF;
    data ^= data << 4;

    return ((((uint16_t)data << 8) | ((crc & 0xFF00) >> 8)) ^ \
            (uint8_t)(data >> 4) ^ \
            ((uint16_t)data << 3));
}


static void bootloader_init(void)
{
        /* Set clock to 32MHz internal RC oscillator. */
        OSC.CTRL = OSC_RC32MEN_bm;

	// Wait for 32MHz osc to become ready
	while((OSC.STATUS & OSC_RC32MRDY_bm) == 0);

	// Now select the int 32MHz as the system clock.
	ccp_write_io(&(CLK.CTRL), OSC_RC2MEN_bm);
	
        /* Initialize the communications interface. */
        twi_init();
}


void main(void)
{        
        /* Initialize the jump to application. */
        void (*start_application)(void) = (void (*)())0;     

        /* Check if we should be here at all - if not we start the application. */
        BOOT_PORT.DIRCLR = BOOT_PIN_MASK;
        if ((BOOT_PORT.IN & BOOT_PIN_MASK)) {
                uint8_t wild_card;
                __EEGET(wild_card, WILDCARD_ADDRESS);
                if (wild_card != BOOTLOADER_WILDCARD) {
                        /* Change line */
                        start_application();
                }
        }   
        
        bootmemory.structured.page_size = APP_PAGE_SIZE;
        bootmemory.structured.page_count = APP_PAGE_CNT;
        
        
        /* Initialize the necessary peripheral modules to run the bootloader. */
        bootloader_init();
        
        /* Endless application loop. */
        while (true) {
          
                /* Poll the TWI bus to see if something has been received. */
                (uint8_t)twi_poll_bus();
                
                /* Check if a new command has been issued. */
                uint8_t cmd = bootmemory.structured.cmd;
                switch (cmd) {
                        case CMD_APP_EXEC:
                                /* Clear the bootloader criteria - goto reset vector */
                                __EEPUT(0, 0x00);
                        case CMD_RESET:
                                /* Do software trigged reset */
                                ccp_write_io(&(RST.CTRL), RST_SWRST_bm);
                        break;
                        case CMD_ERASE_APP:
                                SP_WaitForSPM();
                                SP_EraseApplicationSection();
                                SP_WaitForSPM();
                        break;
                        case CMD_PROGRAM_FLASH:
                                page_address = bootmemory.structured.address;
                                
                                if (page_address < APP_PAGE_CNT) {
                                        page_address <<= ADDRESS_SHIFT;
                        
                                        /* Take content in page buffer and program */
                                        SP_WaitForSPM();
                                        SP_EraseFlashBuffer();
                                        SP_WaitForSPM();
                                        SP_LoadFlashPage(&bootmemory.structured.page[0]);
                                        SP_WriteApplicationPage(page_address);
                                }
                        break;
                        case CMD_PROGRAM_EEPROM:
                                /* Take content in page buffer and program eeprom. */
                                temp_w = bootmemory.structured.page_bytes;
                                page_address = bootmemory.structured.address;
                                temp_ptr = &bootmemory.structured.page[0];
                                while (temp_w != 0) {
                                        __EEPUT(page_address, *temp_ptr);
                                        ++temp_ptr;
                                        ++page_address;
                                        --temp_w;
                                }
                        break;
                        case CMD_FLASH_CRC:
                                crc = 0;
                                temp_dw = 0;
                                
                                while (temp_dw < APPLICATION_SIZE) {
                                        temp_b = SP_ReadByte(temp_dw);
                                        ++temp_dw;
                                        
                                        crc = crc_ccitt_update(crc, temp_b);
                                }
                                
                                bootmemory.structured.crc = crc;
                        break;
                        case CMD_EEPROM_CRC:
                                crc = 0;
                                temp_w = 0;
                                
                                while (temp_dw < EEPROM_SIZE) {
                                        __EEGET(temp_b, temp_dw);
                                        ++temp_dw;
                                        
                                        crc = crc_ccitt_update(crc, temp_b);
                                }
                                
                                bootmemory.structured.crc = crc;
                        break;
                        default: break;
                }
                
                bootmemory.structured.cmd = CMD_NONE;
        }
}


void twi_write_reg(uint16_t address, uint8_t value)
{
        /* Check that we do not access out of allocated memory. */
        if (address >= (sizeof(struct bootloader_memory_map))) {
                return;
        }
        
        if (address >= 6) {
                bootmemory.direct[address] = value;
        } else if (address == 4) {
                bootmemory.direct[5] = value;
        } else if (address == 5) {
                bootmemory.direct[4] = value;
        }
}


uint8_t twi_read_reg(uint16_t address)
{
        return bootmemory.direct[address];
}
