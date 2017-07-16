
/* Pragmas */
// CONFIG1L
#pragma config PLLDIV = 5       // PLL Prescaler Selection bits (Divide by 5 (20 MHz oscillator input))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 2       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes from the 96 MHz PLL divided by 2)

// CONFIG1H
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF     // CCP2 MUX bit (CCP2 input/output is NOT multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are NOT configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

#include <xc.h>

#include "system_config.h"
#include "usb.h"

void I2C_Start(void)
{
    SCL = 0;
    SDA = 0;

    SCL_HIGH();
    SDA_HIGH();
    __delay_ms(I2C_HALF_BIT_DELAY);
    SDA_LOW();
    __delay_ms(I2C_HALF_BIT_DELAY);
}

void I2C_Stop(void)
{
    SCL_HIGH();
    SDA_LOW();
    __delay_ms(I2C_HALF_BIT_DELAY);
    SDA_HIGH();
    __delay_ms(I2C_HALF_BIT_DELAY);
}

bool I2C_Write(uint8_t data)
{
    bool ACK;

    for (i = 0; i < 8; i++)
	{
        SCL_LOW();
        __delay_ms(I2C_HALF_BIT_DELAY / 2);

        if ((data <<= 1) & 0x80)
        SDA_HIGH();
        else
        SDA_LOW();

        __delay_ms(I2C_HALF_BIT_DELAY / 2);
        SCL_HIGH();
        __delay_ms(I2C_HALF_BIT_DELAY);
    }
    SDA_HIGH();
    SCL_LOW();
    __delay_ms(I2C_HALF_BIT_DELAY / 2);
    SCL_HIGH();
    
    i = 0;
    do
    {
        if (i == 10) break;  // Clock stretching for 10 iterations
        ACK = SDA;
        i++;
    }
    while (!ACK);

    SCL_LOW();

    returk ACK;    // if true, transmission NACK
}

uint8_t I2C_Read(void)
{
    uint8_t data;

    SDA_HIGH();

    for (i = 0; i < 8; i++)
	{
        data <<= 1;
        SCL_HIGH();
        if (SDA_IN) data |= 1;
        SCL_LOW();
    }

    if (i != 8) SDA_LOW();   // ACK/NACK bit generation
    else SDA_HIGH();

    SCL_HIGH();
    __delay_ms(I2C_HALF_BIT_DELAY / 2);
    SCL_LOW();
    __delay_ms(I2C_HALF_BIT_DELAY / 2);
    SDA_LOW();

    return data;
}

bool I2C_WriteToSlave(uint8_t devAddress, uint8_t regAddress, uint8_t data)
{
    I2C_Start();
    if (! (I2C_Write(devAddress) && I2C_Write(regAddress) && I2C_Write(data)) )    // checking ACK bits
    I2C_Stop();
    else
    {
        I2C_Stop();
        return false;
    }
    return true;
}

uint8_t I2C_ReadFromSlave(uint8_t devAddress, uint8_t regAddress)
{
    uint8_t readData;

    I2C_Start();
    if (! (I2C_Write(devAddress) && I2C_Write(regAddress)) )
    readData = I2C_Read();
    I2C_Stop;

    return readData;
}

void SPI_Write(unsigned char data)
{
    if(SSPCON1bits.WCOL) SSPCON1bits.WCOL = 0;       	/* Disable collision flag */
    SSPSTATbits.BF=0;                    
    PIR1bits.SSPIF=0;
    PORTBbits.RB1=0;                  /* SCK low */
    SSPBUF = data;                   /* Load data into SSPBUF register */
    PORTBbits.RB1=1;                /* SCK high */
    while(!SSPSTATbits.BF);
    if(SSPCON1bits.SSPOV!=0) SSPCON1bits.SSPOV=0;	         /* Overflow */
}

void MCU_Init(void)
{
	TRISA = 0b00000010;        /* Configured for DATA_REQ */
	TRISB = 0b11110001;        /* Configured for MSSP SPI interface and user interrupts */  /* bit 0 = output, 1 = input */
	TRISC = 0x00;              /* Configured for I2C bit bang */
	INTCON = 0b10001000;       /* Configured for Port B interrupt on change */
	SSPSTAT = 0x00;            /* Transmit on clock transition from idle to active */
	SSPCON = 0x20;             /* SPI clock = Fosc/4 */

    /* USB config */
	UCFG = 0b00000110;         /* Full speed USB (48 MHz) */
                               /* Ping pong buffering enabled for all EP */
	UCONbits.USBEN = 1;        /* Module powered up */
}