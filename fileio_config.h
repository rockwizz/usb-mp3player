
#ifndef _FS_DEF_
#define _FS_DEF_

// Defines system clock frequency for FILEIO library
#define SYS_CLK_FrequencySystemGet 16000000

#define SYS_CLK_FrequencyPeripheralGet SYS_CLK_FrequencySystemGet()

// Defines Instruction Clock frequency for FILEIO library
#define SYS_CLK_FrequencyInstructionGet (SYS_CLK_FrequencySystemGet() / 2)

// Macro indicating how many drives can be mounted simultaneously.
#define FILEIO_CONFIG_MAX_DRIVES        1

// Defines a character to use as a delimiter for directories.  Forward slash ('/') or backslash ('\\') is recommended.
#define FILEIO_CONFIG_DELIMITER '/'

// Macro defining the maximum supported sector size for the FILEIO module.  This value should always be 512 , 1024, 2048, or 4096 bytes.
// Most media uses 512-byte sector sizes.
#define FILEIO_CONFIG_MEDIA_SECTOR_SIZE 		512

/* *******************************************************************************************************/
/************** Compiler options to enable/Disable Features based on user's application ******************/
/* *******************************************************************************************************/

/**********************************************************************
  Define FILEIO_CONFIG_FUNCTION_SEARCH to disable the functions used to
  search for files.                                                    
  **********************************************************************/
//#define FILEIO_CONFIG_SEARCH_DISABLE

// Define FILEIO_CONFIG_FUNCTION_WRITE to disable the functions that write to a drive.  Disabling this feature will
// force the file system into read-only mode.
#define FILEIO_CONFIG_WRITE_DISABLE

// Define FILEIO_CONFIG_FUNCTION_FORMAT to disable the function used to format drives.
//#define FILEIO_CONFIG_FORMAT_DISABLE

// Define FILEIO_CONFIG_FUNCTION_DIRECTORY to disable use of directories on your drive.  Disabling this feature will
// limit you to performing all file operations in the root directory.
//#define FILEIO_CONFIG_DIRECTORY_DISABLE

// Define FILEIO_CONFIG_FUNCTION_DRIVE_PROPERTIES to disable the FILEIO_DrivePropertiesGet function.  This function
// will determine the properties of your device, including unused memory.
//#define FILEIO_CONFIG_DRIVE_PROPERTIES_DISABLE

// Define FILEIO_CONFIG_MULTIPLE_BUFFER_MODE_DISABLE to disable multiple buffer mode.  This will force the library to
// use a single instance of the FAT and Data buffer.  Otherwise, it will use one FAT buffer and one data buffer per drive
// (defined by FILEIO_CONFIG_MAX_DRIVES).  If you are only using one drive in your application, this option has no effect.
#define FILEIO_CONFIG_MULTIPLE_BUFFER_MODE_DISABLE

#endif
