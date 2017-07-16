#include <stdint.h>

#include "fileio.h"
#include "fileio_private.h"     // test
#include "system_config.h"
#include "sta013.h"

void STA013_Patch()
{
	/* Prerequisite: Requires a file in root dir PATCH.TXT containing
					vendor patch data for STA013 with attribute HIDDEN */

	uint8_t tempData[2];
	FILEIO_SEARCH_RECORD record;

	if (FILEIO_Find( (char*) "PATCH.TXT", FILEIO_ATTRIBUTE_HIDDEN, &record, true) == FILEIO_RESULT_SUCCESS)
	{
		if (FILEIO_Open(&myFile, "PATCH.TXT", FILEIO_OPEN_READ) == FILEIO_RESULT_SUCCESS)
		{
			do
			{
				if (FILEIO_Read(&tempData, 1, 2, &myFile) == 2)   // check objects read
				{
					if (I2C_WriteToSlave(0x86, tempData[1], tempData[2]))
					continue;
				}
			}
			while (!(FILEIO_Eof(&myFile)));
		}
        
	}
}

void STA013_Config()
{
	uint8_t address[9] = {0x06, 0x0B, 0x61, 0x50, 0x65, 0x52, 0x64, 0x51, 0x05};    /* Array of address from Initialization sequence */
	uint8_t data[9] = {0x09, 0x02, 0x05, 0x06, 0x00, 0xB8, 0x00, 0x00, 0xA1};       /* Array of data from Initialization sequence */
	
	/* PLL Configuration sequence for 14.7456 MHz clock 512x oversampling */
	for (int i=0; i< 9; i++)
	{
		if (I2C_WriteToSlave(0x86, address[i], data[i]))
		continue;
	}
}

void STA013_Run()
{
	if (I2C_WriteToSlave(0x86, 0x72, 0x01));
}

void STA013_Init()
{
    STA013_Patch();
    STA013_Config();
    STA013_Run(); 
}