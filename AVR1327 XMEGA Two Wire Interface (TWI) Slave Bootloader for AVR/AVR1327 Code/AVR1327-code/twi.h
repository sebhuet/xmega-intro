#ifndef TWI_H_INCLUDED
#define TWI_H_INCLUDED


#include <stdint.h>
#include <stdbool.h>


enum twi_states {
        TWI_STATE_IDLE,
        TWI_STATE_ERROR,
        TWI_STATE_STOP
};


void twi_init(void);
uint8_t twi_poll_bus(void);

void twi_write_reg(uint16_t address, uint8_t value);
uint8_t twi_read_reg(uint16_t address);
#endif /*TWI_H_INCLUDED*/