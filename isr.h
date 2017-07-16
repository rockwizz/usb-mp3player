#ifndef _ISR_DOT_H
#define _IST_DOT_H

#include <xc.h>

typedef enum
{
	STA013_PLAY,
	STA013_STOP
} STA013_STATE;

/* Prototypes */
// void interrupt UI(void);

void interrupt UI_Init(void);

#endif // _ISR_DOT_H