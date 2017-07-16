/*
Project name: usb_mp3player
File name: main.c
Author: Shreyas R
Target hardware: PIC18F2550
Target compiler: XC8
Date: 23-03-2017
Edited: 08-05-17
*/

/* IMPORTANT: Uses libraries from Microchip Libraries for Applications.
Visit http://www.microchip.com/mplab/microchip-libraries-for-applications */

/* Includes */
#include <xc.h>

#include "system_config.h"
#include "usb.h"
#include "usb_host_msd.h"
#include "fileio_app.h"
#include "mp3player.h"
#include "sta013.h"
#include "isr.h"

/* Globals */
uint8_t i;

/* Prototypes */
void FILEIO_GetTimestamp(FILEIO_TIMESTAMP * timestamp);

bool USB_EventHandler(uint8_t address, USB_EVENT event, void * pdata, uint16_t size);
bool FILEIO_EventHandler(FILEIO_ERROR_TYPE event);

void __attribute__((interrupt,auto_psv)) _USB1Interrupt()
{
    USB_EventHandler();
}

/* Return value for eventhandlers: */
/* If event handled = return true */
/* If event not handled or fatal error = return false */

bool USB_EventHandler(uint8_t address, USB_EVENT event, void * pdata, uint16_t size)
{
    switch( (int) event)	// why is _event_ typecast as int type?
    {
        case USB_SUCCESS:
			return true;
			break;
        case EVENT_VBUS_REQUEST_POWER:
            return true;
        case EVENT_VBUS_RELEASE_POWER:
			state = WAITING_FOR_ATTACH;
            return true;
            break;
        case EVENT_HUB_ATTACH:
			return true;
        case EVENT_UNSUPPORTED_DEVICE:
        case EVENT_CANNOT_ENUMERATE:
        case EVENT_CLIENT_INIT_ERROR:
        case EVENT_OUT_OF_MEMORY:
        case EVENT_UNSPECIFIED_ERROR:
			UCON.USBEN = 0;				/* Attempting a soft h/w reset of USB module */
			UCON.USBEN = 1;
			USBHostInit(0);
			if (USBGetDeviceState() < CONFIGURED_STATE)
            {	return true;	}
			else return false;
            break;
        case EVENT_MSD_ATTACH:

        default:
        	break;
    }
    return false;
}

bool FILEIO_EventHandler(FILEIO_ERROR_TYPE event)
{
	switch(event)
	{
		case FILEIO_RESULT_SUCCESS:
			return true;
			break;
		case FILEIO_RESULT_FAILURE:
			return false;
		case FILEIO_ERROR_NONE:
			return true;
			break;
		case FILEIO_ERROR_EOF:
			FILEIO_Close(&myFile);
			filename = FILEIO_FileHandler(NEXT_FILE, 0);
			return true;
		case FILEIO_ERROR_DONE:
			FILEIO_DriveHandler(UNMOUNT);
			return true;
			break;
		case FILEIO_ERROR_BAD_SECTOR_READ:
			return true;						// So the program can proceed
		case FILEIO_ERROR_NOT_PRESENT:
        	state = WAITING_FOR_ATTACH;
			return true;
			break;
		default:
			break;
	}
	return false;
}

void FILEIO_GetTimestamp(FILEIO_TIMESTAMP * timestamp)
{
	/* Leave some defaults here if system doesn't use an RTC */
    timestamp->date.bitfield.day = 14;
    timestamp->date.bitfield.month = 3;
    timestamp->date.bitfield.year = 12;
    timestamp->time.bitfield.hours = 5;
    timestamp->time.bitfield.secondsDiv2 = 10;
    timestamp->time.bitfield.minutes = 1;
    timestamp->timeMs = 40;
}

int main(void)
{
	/* Check order */
	MCU_Init();
	STA013_Run();
	FILEIO_Initialize();
	FILEIO_RegisterTimestampGet(FILEIO_GetTimestamp);
    USBHostInit(0);
	MP3PLAYER_Init();											// sets WAITING_FOR_ATTACH flag
	FILEIO_DriveHandler(MOUNT);
	FILEIO_FileHandler(NULL, true);
	UI_Init();													// sets the STA013_PLAY flag

    for(;;)
    {
		while (DATA_REQ)
		{
			USBTasks();

      		MP3PLAYER_Tasks();

			CLRWDT();		// make sure WDT frequency > 1 / cycles of (USBTasks() + mp3player_tasks() + ISR)
		}
	}
	return 0;
}

