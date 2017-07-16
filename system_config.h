#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_

/* Includes */
#include <xc.h>
#include <stdint.h>
#include<stdbool.h>

#include "usb_config.h"

/* Defines */
#define _XTAL_FREQ 20000000                  // Frequency in Hz

#define I2C_SPEED 10000                      // Baud rate in bps
                                             // Max I2C_SPEED is 100000 bps @ _XTAL_FREQ 20 MHz

#define I2C_HALF_BIT_DELAY (500 / I2C_SPEED) // I2C half bit delay = (1/I2C_SPEED) / (1000/2)

#define SCL PORTCbits.RC1
#define SDA PORTCbits.RC0

/* I2C macros */
#define SDA_HIGH()                \
    {                             \
        TRISCbits.TRISC0 = 1;     \
    }

#define SDA_LOW()                 \
    {                             \
        TRISCbits.TRISC0 = 0;     \
    }

#define SCL_HIGH()                \
    {                             \
        TRISCbits.TRISC1 = 1;     \
    }

#define SCL_LOW()                 \
    {                             \
        TRISCbits.TRISC1 = 0;     \
    }

// void __attribute__((interrupt,auto_psv)) _AltUSB1Interrupt();   // check/fix this!

/* Prototypes */
void I2C_Start(void);                   /* Signals START condition on I2C bus */
void I2C_Stop(void);                    /* Signals STOP condition on I2C bus */
void I2C_Send(uint8_t data);            /* Sends a byte on I2C bus, this can be address of slave or data */
uint8_t I2C_Read(bool NACK);            /* Reads a byte from slave on I2C bus */

bool I2C_WriteToSlave(uint8_t devAddress, uint8_t regAddress, uint8_t data);     // only these are
uint8_t I2C_ReadFromSlave(uint8_t devAddress, uint8_t regAddress);               // implemented by user

void SPI_Write(unsigned char data);     /* Uses MSSP configured for SPI to send data */

void MCU_Init(void);

#endif