
/* Includes */
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

#include "usb.h"
#include "fileio.h"
#include "mp3player.h"
#include "system_config.h"
#include "fileio_app.h"

static FILEIO_OBJECT myFile;

static USB_HOST_MSD_STATE state;
static uint8_t deviceAddress = 0;

// Declare a FILEIO_DRIVE_CONFIG structure to describe which functions the File I/O library will use to communicate with the media
const FILEIO_DRIVE_CONFIG gUSBDrive =
{
    (FILEIO_DRIVER_IOInitialize)NULL,									  // Function to initialize the I/O pins used by the driver.
    (FILEIO_DRIVER_MediaDetect)USBHostMSDSCSIMediaDetect,                 // Function to detect that the media is inserted.
    (FILEIO_DRIVER_MediaInitialize)USBHostMSDSCSIMediaInitialize,         // Function to initialize the media.
    (FILEIO_DRIVER_MediaDeinitialize)USBHostMSDSCSIMediaDeinitialize,     // Function to de-initialize the media.
    (FILEIO_DRIVER_SectorRead)USBHostMSDSCSISectorRead,                   // Function to read a sector from the media.
    (FILEIO_DRIVER_SectorWrite)USBHostMSDSCSISectorWrite,                 // Function to write a sector to the media.
    (FILEIO_DRIVER_WriteProtectStateGet)USBHostMSDSCSIWriteProtectState,  // Function to determine if the media is write-protected.
};

uint8_t MP3Data;
uint8_t readObjects;

void MP3PLAYER_Init()
{
    state = WAITING_FOR_ATTACH;
}

void MP3PLAYER_Tasks()
{
    if((USBGetDeviceState() < CONFIGURED_STATE) || (USBIsDeviceSuspended() == true))
    {
        continue;
    }
    else
    {
        if (FILEIO_EventHandler(FILEIO_GetError('A')))
        // handle any FILEIO errors before Reading from file
        {
			readObjects = FILEIO_Read(&MP3Data, 1, 5, &myFile);			    /* No. of objects set at 5 */
																		    /* consider increasing object count if DATA_REQ instances occur frequently */
            for (i=0; i<= readObjects; i++)
            {
                PORTAbits.RA4 = 1;    // activity indicator LED
                if (MP3Data[i])												/* Check if data present */
                {
                    SPI_Write(MP3Data[i]);
                }
                PORTAbits.RA4 = 0;
            }
        }
    }
}