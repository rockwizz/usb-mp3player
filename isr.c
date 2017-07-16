#include <xc.h>
#include <stdint.h>

#include "isr.h"
#include "system_config.h"
#include "fileio.h"
#include "fileio_app.h"

static STA013_STATE status;

void UI_Init(void)
{
    status = STA013_PLAY;
}

 void interrupt UI(void)
/* High priority ISR */
{
	uint8_t volume;

	if (INTCONbits.RBIE && INTCONbits.RBIF)
	{
		INTCONbits.RBIF = 0;

		/* STOP */
		if (LATBbits.LATB4 && (status == STA013_PLAY))
		{
			I2C_WriteToSlave(0x86, 0x13, 0x00);
			status = STA013_STOP;
			LATAbits.LATA0=0;
		}

		/* PLAY */
		else if (LATBbits.LATB4 && (status == STA013_STOP))
		{
			I2C_WriteToSlave(0x86, 0x13, 0x01);
			status = STA013_PLAY;
			LATAbits.LATA0=0;
		}

		/*  VOL+ */
		else if (LATBbits.LATB5)
		{
			/* Assuming both DLA and DRA have the same initial value,
			   reading from one is sufficient to increment volume */
			volume = I2C_ReadFromSlave(0x86, 0x46);

			if (!(volume > 0x5A))
			{
				I2C_WriteToSlave(0x86, 0x46, volume + 0x0A);
				I2C_WriteToSlave(0x86, 0x46, volume + 0x0A);
			}

			LATBbits.LATB5 = 0;
		}

		/* VOL- */
		else if (LATBbits.LATB6)
		{
			volume = I2C_ReadFromSlave(0x86, 0x46);

			if (!(volume > 0x5A))
			{
				I2C_WriteToSlave(0x86, 0x46, volume - 0x0A);
				I2C_WriteToSlave(0x86, 0x46, volume - 0x0A);
			}

			LATBbits.LATB6 = 0;
		}

		/* Next track */
		else if (LATBbits.LATB7)
		{
			if (FILEIO_Close(&myFile) == FILEIO_RESULT_SUCCESS)
			{
    			if (FILEIO_FileHandler(NEXT_TRACK, *filename, filenames[], 0))
				{
					LATBbits.LATB7 = 0;
				}
			}
		}

		/* TODO: Write else if PREV_TRACK and add supporting h/w */

		/* Power down */
		else if (LATBbits.LATB5 && LATBbits.LATB6)
		{
			if ((FILEIO_Close(&myFile) == FILEIO_RESULT_SUCCESS);
			{
				if (FILEIO_DriveHandler(UNMOUNT))
				{
					status = WAITING_FOR_DETACH;
					
					LATBbits.LATB5 = 0;						/* Clear latch bits if operation successful */
					LATBbits.LATB6 = 0;
					
					I2C_WriteToSlave(0x86, 0x13, 0x00);		/* Stop STA013 so DATA_REQ does not occur */
				}
			}
		}
	}
}