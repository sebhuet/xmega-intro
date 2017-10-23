/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */
#ifndef ARCH_ATXMEGA_CCP_H_INCLUDED
#define ARCH_ATXMEGA_CCP_H_INCLUDED


#include <stdint.h>


void ccp_write_io(uint8_t volatile *reg, uint8_t value);
void ccp_unprotected_write_io(uint8_t volatile *reg, uint8_t value);
#endif /*ARCH_ATXMEGA_CCP_H_INCLUDED*/
