#ifndef _STA013_DOT_H
#define _STA013_DOT_H

#include <xc.h>

#define DATA_REQ PORTAbits.RA1

/* Prototypes */
void STA013_Init(void);
void STA013_Config(void);
void STA013_Run(void);
void STA013_Patch(void);

#endif