#include "usb.h"
#include "usb_host_msd.h"
#include "usb_host_msd_scsi.h"

// *****************************************************************************
// Media Interface Function Pointer Table for the Mass Storage client driver
// *****************************************************************************

CLIENT_DRIVER_TABLE usbMediaInterfaceTable =
{                                           
    USBHostMSDSCSIInitialize,
    USBHostMSDSCSIEventHandler,
    0
};

// *****************************************************************************
// Client Driver Function Pointer Table for the USB Embedded Host foundation
// *****************************************************************************

CLIENT_DRIVER_TABLE usbClientDrvTable[] =
{                                        
    {
        USBHostMSDInitialize,
        USBHostMSDEventHandler,
        0
    }
};

// *****************************************************************************
// USB Embedded Host Targeted Peripheral List (TPL)
// *****************************************************************************

USB_TPL usbTPL[] =
{
    { INIT_CL_SC_P( 8ul, 6ul, 0x50ul ), 0, 0, {TPL_CLASS_DRV} } // Most types of MSD flash drives
    ,
    { INIT_CL_SC_P( 8ul, 5ul, 0x50ul ), 0, 0, {TPL_CLASS_DRV} } // Some MSD flash drives use this instead
};



