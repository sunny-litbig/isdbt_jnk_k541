/*
 * Copyright (C) 2012 Telechips, Inc.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <stdio.h>

#include "OMX_Core.h"
#include "OMX_Component.h"
#include "OMX_Types.h"

#include "isdbt_tuner.h"

#include "linuxtv_utils.h"

#include "dtvDump.h"

#define PRINTF     printf
#define MBYTE      1024*1024

struct dtvConfig {
	char *pFileName;
	int iFileSize;
	int iStandard;
	int iTSIF_Ch;
	int iFreqKHz;
	int iBWHz;
	int iPIDs[256];
	int iPIDNum;
};

static void dtvDump(struct dtvConfig *pdtvConfig)
{
	OMX_COMPONENTTYPE *pOpenmaxStandComp;
	LINUXTV *pLinuxTV;
	FILE *pfp;
	char *p;
	int iWrittenLen = 0;
	int iRet;

	///////////////////////////////////////////
	// Tuner Open /////////////////////////////
	///////////////////////////////////////////
	switch (pdtvConfig->iStandard)
	{
	case 0:
		pOpenmaxStandComp = isdbt_tuner_open(pdtvConfig->iFreqKHz, pdtvConfig->iBWHz);
		break;
	default:
		pOpenmaxStandComp = NULL;
		break;
	}

	if (pOpenmaxStandComp == NULL)
		return;
	///////////////////////////////////////////

	///////////////////////////////////////////
	// Recording //////////////////////////////
	///////////////////////////////////////////
	printf("==================================================\n");
	printf("== dtvDump : START ==\n\n");

	pfp = fopen(pdtvConfig->pFileName, "wb");
	if (pfp != NULL)
	{
		pLinuxTV = linuxtv_open(pdtvConfig->iTSIF_Ch, pdtvConfig->iPIDs, pdtvConfig->iPIDNum);
		if (pLinuxTV != NULL)
		{
			if (linuxtv_start(pLinuxTV) == 0)
			{
				p = malloc(MBYTE);
				if (p != NULL)
				{
					system("echo performance>/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");

					while(pdtvConfig->iFileSize == 0 || iWrittenLen < pdtvConfig->iFileSize)
					{
						iRet = linuxtv_read(pLinuxTV, p, MBYTE);
						if (iRet < 0)
							break;

						if (iRet > 0)
						{
							iRet = fwrite(p, 1, MBYTE, pfp);
							if (iRet <= 0)
								break;

							iWrittenLen++;
		
							printf("== dtvDump : %d/%d MB\n", iWrittenLen, pdtvConfig->iFileSize);
						}
					}

					printf("=========================\n");
					printf("Dump Size : %d MB\n", iWrittenLen);
					printf("=========================\n");
	
					system("echo interactive>/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");

					free(p);
				}
				linuxtv_stop(pLinuxTV);
			}
			linuxtv_close(pLinuxTV);
		}
		fclose(pfp);
	}

	printf("\n== dtvDump : END ==\n");
	printf("==================================================\n");
	///////////////////////////////////////////

	///////////////////////////////////////////
	// Tuner Close ////////////////////////////
	///////////////////////////////////////////
	switch (pdtvConfig->iStandard)
	{
	case 0:
		isdbt_tuner_close(pOpenmaxStandComp);
		break;
	}
	///////////////////////////////////////////
}

static void Print_Usage()
{
	printf(	"Usage : dtvDump filename filesize tsifchannel freq bw [pids]\n"
			"        filename\n"
			"        filesize - MB\n"
			"        tsifchannel - tsif channel\n"
			"        frequency - kHz\n"
			"        bw - Hz\n"
			"        pids\n");
}

int main(int argc, char *argv[])
{
	struct dtvConfig dtvConfig;

	if (argc < 6)
	{
		Print_Usage();
		return 0;
	}

	memset(&dtvConfig, 0x0, sizeof(struct dtvConfig));

	while (argc > 6)
	{
		dtvConfig.iPIDs[dtvConfig.iPIDNum] = atoi(argv[--argc]);
		dtvConfig.iPIDNum++;
	}

	if (dtvConfig.iPIDNum == 0)
	{
		dtvConfig.iPIDs[dtvConfig.iPIDNum] = 0x2000;
		dtvConfig.iPIDNum++;
	}

	dtvConfig.iBWHz     = atoi(argv[--argc]);
	dtvConfig.iFreqKHz  = atoi(argv[--argc]);
	dtvConfig.iTSIF_Ch  = atoi(argv[--argc]);
	dtvConfig.iFileSize = atoi(argv[--argc]);
	dtvConfig.pFileName = argv[--argc];

	dtvDump(&dtvConfig);

	return 0;
}