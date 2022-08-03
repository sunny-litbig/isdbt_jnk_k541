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


#ifndef	_TSPARSE_ISDBT_H_
#define	_TSPARSE_ISDBT_H_

#ifdef __cplusplus
extern "C" {
#endif
    
#include "TsParse.h"
#include "mpeg_ts.h"
#include "mpeg_def.h"
#include "mpeg_ext.h"
#include "mpegpars.h"
#include "mpegpars_DVB.h"
#include "BitParser.h"

enum
{
	SERVICE_TYPE_FSEG		= 0x01,
	SERVICE_TYPE_TEMP_VIDEO	= 0xA1,
	SERVICE_TYPE_1SEG		= 0xC0
};
#define MAX_SUPPORT_CURRENT_SERVICE	32

void ISDBT_TSPARSE_Init(void);
void ISDBT_TSPARSE_Deinit(void);    // Noah / 20180705 / Added for IM478A-51 (Memory Access Timing)

int ISDBT_TSPARSE_PES_data_field (void *handle, unsigned char *pucRawData, int iLenData);
int ISDBT_TSPARSE_ProcessTable (void *handle, unsigned char *pucRawData, unsigned int uiRawDataSize, int RequestID);
void ISDBT_Init_DescriptorInfo(void);
void ISDBT_Init_CA_DescriptorInfo(void);
void ISDBT_TSPARSE_ServiceList_Init (void);

int isdbt_emergency_info_get_data(int *service_id, int *area_code, int *signal_type, int *start_end_flag,  int *area_code_length);
void isdbt_emergency_info_clear(void);
int isdbt_mvtv_info_get_data(int iServiceID, int index, unsigned char *pucComponentTag, int *piComponentCount);
extern int ISDBT_TSPARSE_CURDESCINFO_set(unsigned short usServiceID, unsigned char ucDescID, void *pvDesc);
extern int ISDBT_TSPARSE_CURDCCDESCINFO_get(unsigned short usServiceID, void **pDCCInfo);
extern int ISDBT_TSPARSE_CURDESCINFO_get(unsigned short usServiceID, DESCRIPTOR_IDS ucDescID, void *pvDesc);
extern int ISDBT_TSPARSE_CURDESCINFO_setBitMap(unsigned short usServiceID, unsigned char ucTableID, unsigned char uciDescBitMap);
int ISDBT_GetTOTMJD(void);
int UpdateDB_SpecialService_DelInfo (void);
int UpdateDB_SpecialService_UpdateInfo (int channel_number, int country_code, unsigned short service_id, int *row_id);

extern int ISDBT_Get_1SegServiceCount(void);
extern int ISDBT_Get_TotalServiceCount(void);

#ifdef __cplusplus
}
#endif
#endif	//_TSPARSE_ISDBT_H_

