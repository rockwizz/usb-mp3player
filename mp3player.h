#ifndef _MP3_PLAYER_DOT_H
#define _MP3_PLAYER_DOT_H

#include "usb_host_msd_scsi.h"	// contains SCSI commands, just an FYI

typedef enum
{
    WAITING_FOR_ATTACH,
    WAITING_FOR_DETACH
} USB_HOST_MSD_STATE;

void MP3PLAYER_Init();
void MP3PLAYER_Tasks();

#endif