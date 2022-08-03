/*
 * Copyright (C) 2013 Telechips, Inc.
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

#ifndef __SUBTITLE_DEBUG_H__
#define __SUBTITLE_DEBUG_H__

#define LOGE		ALOGE
#define LOGD		ALOGD
#define LOGI		ALOGI
#define LOGW		ALOGW
#define LOGV		ALOGV

#define CC_PRINTF	//LOGE
#define ERR_DBG		LOGE

//#define SUBTITLE_DATA_DUMP


#if defined(SUBTITLE_DATA_DUMP)
void dump_data_group(TS_PES_CAP_DATAGROUP	*ptrCurrDGroup, int iDGroupNum)
{
	TS_PES_CAP_DATAUNIT 	*ptrCurrDUnit, *ptrNextDUnit;
	int j, iDataUnitSize;
	char szDumpBuf[100]={0,}, szDummy[5]={0,};
	
	while (ptrCurrDGroup != NULL)
	{
		if (iDGroupNum == 0)
		{
			break;
		}
		iDGroupNum--;

		/* Check a caption satatement data */
		if (ptrCurrDGroup->DState.DUnit != NULL)
		{
			ptrCurrDUnit = (TS_PES_CAP_DATAUNIT *)ptrCurrDGroup->DState.DUnit;
			while (ptrCurrDUnit != NULL)
			{
				ptrNextDUnit = (TS_PES_CAP_DATAUNIT *)ptrCurrDUnit->ptrNextDUnit;
				iDataUnitSize = (int)ptrCurrDUnit->data_unit_size;

				LOGE("===================================================\n");
				LOGE("_DATA_DUMP_OF_DATA_UNIT(STATEMENT:%d)\n", iDataUnitSize);
				for (j = 0; j < ptrCurrDUnit->data_unit_size; j++)
				{
					if((j!=0)&&((j%16)==0)){
						LOGE("%s\n", szDumpBuf);
						memset(szDumpBuf, 0x0, 100);
					}
					sprintf(szDummy, "0x%02X ", ptrCurrDUnit->pData[j]);
					strcat(szDumpBuf, szDummy);
					
				}
				if(iDataUnitSize%16){
					LOGE("%s\n", szDumpBuf);
				}
				LOGE("\n===================================================\n");

				ptrCurrDUnit = ptrNextDUnit;
			}
		}

		/* Check a caption management data. */
		if (ptrCurrDGroup->DMnge.DUnit != NULL)
		{
			ptrCurrDUnit = (TS_PES_CAP_DATAUNIT *)ptrCurrDGroup->DMnge.DUnit;
			while (ptrCurrDUnit != NULL)
			{
				ptrNextDUnit = (TS_PES_CAP_DATAUNIT *)ptrCurrDUnit->ptrNextDUnit;
				iDataUnitSize = (int)ptrCurrDUnit->data_unit_size;
				
				LOGE("===================================================\n");
				LOGE("_DATA_DUMP_OF_DATA_UNIT(MANAGEMENT:%d)\n", iDataUnitSize);
				for (j = 0; j < ptrCurrDUnit->data_unit_size; j++)
				{
					if((j!=0)&&((j%16)==0)){
						LOGE("%s\n", szDumpBuf);
						memset(szDumpBuf, 0x0, 100);
					}
					sprintf(szDummy, "0x%02X ", ptrCurrDUnit->pData[j]);
					strcat(szDumpBuf, szDummy);
				}
				if(iDataUnitSize%16){
					LOGE("%s\n", szDumpBuf);
				}
				LOGE("\n===================================================\n");
				
				ptrCurrDUnit = ptrNextDUnit;
			}
		}

		ptrCurrDGroup = (TS_PES_CAP_DATAGROUP *)ptrCurrDGroup->ptrNextDGroup;
	}
}

void dump_data_unit(char *pType, TS_PES_CAP_DATAUNIT *ptrCurrDUnit, int iDataUnitSize)
{
	char szDumpBuf[100]={0,}, szDummy[5]={0,};
	int j, cnt=0;
	
	LOGE("===================================================\n");
	LOGE("_DATA_DUMP_OF_DATA_UNIT_____%s______(0x%02X:%d)", \
		pType, ptrCurrDUnit->data_unit_parameter, iDataUnitSize);

	cnt = (iDataUnitSize/16);
	if(iDataUnitSize%16){
		cnt++;
	}
	
	for (j = 0; j < iDataUnitSize; j++)
	{
		if((j!=0)&&((j%16)==0)){
			LOGE("%s\n", szDumpBuf);
			memset(szDumpBuf, 0x0, 100);
			cnt--;
		}
		sprintf(szDummy, "0x%02X ", ptrCurrDUnit->pData[j]);
		strcat(szDumpBuf, szDummy);
	}
	if(cnt){
		LOGE("%s\n", szDumpBuf);
	}
	LOGE("\n===================================================\n");
}
#endif /* SUBTITLE_DATA_DUMP */

#endif	/* __SUBTITLE_DEBUG_H__ */