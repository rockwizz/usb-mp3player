#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "fileio_private.h"
#include "fileio.h"
#include "fileio_app.h"

/* Application functions */

bool FILEIO_DriveHandler(FILEIO_DRIVE_CMD driveCmd)
{
    /* Edited on your birthday */
    /* +*+*+*+ HAPPY __*__ BIRTHDAY +*+*+*+ */

    /* Max no. of devices set at 1, so only drive ID 'A' is used */

    switch (state)
    {
        case WAITING_FOR_ATTACH:
        if (driveCmd == MOUNT)
        {
            if (FILEIO_MediaDetect(&gUSBDrive, &deviceAddress) == true)
            {
                if (!(FILEIO_DriveMount ('A', &gUSBDrive, &deviceAddress) == FILEIO_ERROR_NONE))
                {
                    /* Drive mount failed */
                    state = WAITING_FOR_DETACH;
                    return false;
                }
                else return true;
            }
        }
        else if (driveCmd == UNMOUNT)
        {
            FILEIO_DriveUnmount ('A');
            state = WAITING_FOR_DETACH;
            return true;
        }
        else ;
        case WAITING_FOR_DETACH:
        break;
        default: break;
    }
}

bool FILEIO_FileHandler(FILEIO_FILE_CMD fileCmd, bool init)
{
    /* Prerequisite: Requires a global pointer to element in filenameArray */

    if (init)
    {
        if (FILEIO_FilenamesIndexer())
        {
            *fileName = &fileNameArray[0][];   
            return true;
        }
        return false;   // filename indexing operation failed
    }
    // currentFileName = FILEIO_CacheDirectory(&directory, *filename, false);

    switch (fileCmd)
    {
        case NEXT_TRACK:
        
        for (i=0; i<= filenameArray->fileCount; i++)
        {
            if (strcmp (* fileName, filenameArray[filenameArray[i]][filenameArray->filenameCharacterCount]))
            {
                * fileName = & filenameArray[i + 1][]; // set pointer to required address
                break;
            }
        }
        case PREV_TRACK:

        for (i= filenameArray->fileCount; i>= filenameArray->fileCount; i--)
        {
            if (strcmp (* fileName, filenameArray[i][filenameArray->filenameCharacterCount]))
            {
                * fileName = & filenameArray[i - 1][filenameArray->filenameCharacterCount];
                break;
            }
        }
        default: break;
    }
    
    return true; // file operation handled successfully
}

bool FILEIO_FilenamesIndexer()
{
    /* Prerequisite: FILEIO libraray should be initialized and drive mounted */

    uint8_t tempData[drive->sectorSize];
    uint32_t tempAddress;
    unsigned char tempFilename[11];         // short filename only
    uint8_t j, k, l;                        // loop variables

    filenameArray->fileCount = 0;

    switch (drive->type)
    {
        case FILEIO_FILE_SYSTEM_TYPE_FAT32:
        for (i=2; i<= drive->partitionClusterCount; i++)    // first 2 clusters not used
        {
            tempAddress = drive->firstCluster + ((i-2) * drive->sectorsPerCluster);     // first cluster LBA
        
            if (USBHostMSDSCSISectorRead(tempAddress, &tempData))   // reads one sector
            {
                for (j=0; j<= drive->sectorsPerCluster, i++)
                {
                    for (k=0; k<= (drive->sectorSize / 32); k++)
                    {
                        switch tempData[k * 32]
                        {
                            case 0xE5: continue;
                            case 0x00: return true;
                            default:
                            if (((tempData[12] >> 1) & 0) || ((tempData[12] >> 2) & 0) || ((tempData[12] >> 3) & 0) || ((tempData[12] >> 4) & 0))
                            // To check attrib bits if ! (HIDDEN | SYSTEM | VOLUME ID | SUBDIR)
                            /*****************************************************************************
			                NOTE ON SHORT CIRCUITING:
				                6.5.14 Logical OR operator
					                (4). Unlike the bitwise binary & operator, the && operator guarantees
					                left-to-right evaluation; there is a sequence point after the
					                evaluation of the first operand. If the first operand compares equal
					                to 0, the second operand is not evaluated.
			                ******************************************************************************/
                            {
                                for (l=0; l<= 11; l++)   // short filename + attrib byte
                                {
                                    if ((l <= 8) && (tempData[l] != '0'))            // filename
                                                                                     // remove padded zeroes before concatenating
                                    {   strcat(tempFilename, (char) tempData[l]);   }
                                    else
                                    if ((l >= 8) && (tempData[l] != '0'))
                                    {
                                        strcat(tempFilename, '.');
                                        strcat(tempFilename, tempData[l]);         // extension
                                    }
                                    // else ;          
                                }
                                if (tempFilename != '0')
                                {
                                    strcpy(filenameArray[filenameArray->fileCount++][filenameArray->filenameCharacterCount], tempFilename);
                                    // edited while enjoying Royal Challenge deluxe whisky |_| <- tumbler
                                }
                                else return false;  // filename indexing operation failed
                            }
                            else continue;  // not valid file, skipping to next iteration
                        }
                    }
                }
            }
            else return false;
        }

        case FILEIO_FILE_SYSTEM_TYPE_FAT16:
        for (i=0; i<= disk->rootDirectoryEntryCount; i++)
        {
            tempAddress = drive->firstRootSector + (drive->sectorSize * i);
            if ((USBHostMSDSCSISectorRead(tempAddress, &tempData)))
            {
                for (j=0; j<= (drive->sectorSize / 32); j++)
                {
                    switch tempData[j * 32]
                    {
                        case 0xE5: continue;
                        case 0x00: return true;
                        default:
                        if (((tempData[12] >> 1) & 0) || ((tempData[12] >> 2) & 0) || ((tempData[12] >> 3) & 0) || ((tempData[12] >> 4) & 0))
                                                                             // check attrib bits if ! (HIDDEN | SYSTEM | VOLUME ID | SUBDIR)
                        {
                            for (l=0; l<= 11; l++)   // short filename + attrib byte
                            {
                                if ((l <= 8) && (tempData[l] != '0'))            // filename
                                                                                 // remove padded zeroes before concatenating
                                {   strcat(tempFilename, (char) tempData[l]);   }
                                else
                                if ((l >= 8) && (tempData[l] != '0'))
                                {
                                    strcat(tempFilename, '.');
                                    strcat(tempFilename, tempData[l]);         // extension
                                }
                                // else ;          
                            }
                            if (tempFilename != '0')
                            {
                                strcpy(filenameArray[filenameArray->fileCount++][filenameArray->filenameCharacterCount], tempFilename);
                            }
                            else return false;  // filename indexing operation failed
                        }
                        else continue;  // not valid file, skipping to next iteration
                    }
                }
                if (! (tempAddress <= drive->firstDataSector) )
                {   continue;   }
                else return true;       // file operation completed
            }
            else return false;
        }

        case FILEIO_FILE_SYSTEM_TYPE_FAT12:
        /* In this day and age - never used */
        break;

        default:
        /* Unsupported file system */        
        return false;
    }
    return true;
}
