#ifndef  _FILEIO_APP_DOT_H
#define  _FILEIO_APP_DOT_H

#include <stdint.h>
#include <stdbool.h>

#include "fileio.h"

/* Globals */
unsigned char *fileName;

typedef struct
{
    uint8_t fileCount;
    uint8_t filenameCharacterCount;   // short filename only
    void * filenameArray;
} filenameArrayProperties;

unsigned char filenameArray[filenameArray->fileCount][filenameArray->filenameCharacterCount];

/* Enums and structs */
typedef enum
{
    NEXT_TRACK,
    PREV_TRACK
} FILEIO_FILE_CMD;

typedef enum
{
    MOUNT,
    UNMOUNT
} FILEIO_DRIVE_CMD;

/* Function prototypes */
bool FILEIO_FileHandler(FILEIO_FILE_CMD fileCmd, bool init));
/***************************************************************************
  Function:
    bool FILEIO_fileHandler(FILEIO_FILE_CMD fileCmd, unsigned char *fileName, unsigned char filenameArray, bool init))

    Summary:
        Handles app file operations such as NEXT_TRACK and PREV_TRACK

    Description:
        This function will read and index (if required) filenames from a
        FAT 12/16/32 file system. The indexed filenames are used to
        perform application-specific file operations such as NEXT_TRACK and
        PREV_TRACK.

    Precondition:
        FILEIO library should be initiialized and a drive should be
        mounted. A pointer to some element in array containing filenames
        and the array itself is declared.

    Parameters:
        fileCmd - object of enum FILEIO_FILE_CMD
        *fileName - pointer to element in array containing filenames
        filenameArray[] - array of strings containing filenames
        init - set when FILEIO_fileHandler called for the first time

    Returns:
      * If Success: true
      * If Failure: false
                              
  ****************************************************************************/

bool FILEIO_DriveHandler(FILEIO_DRIVE_CMD driveCmd);

bool FILEIO_FilenamesIndexer();

#endif
