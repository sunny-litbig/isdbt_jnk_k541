/****************************************************************************

Copyright (C) 2013 Telechips Inc.


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions andlimitations under the License.

****************************************************************************/

/****************************************************************************

Revision History

****************************************************************************

****************************************************************************/
#ifndef _TCC_LINUXTV_SYSTEM_H_
#define _TCC_LINUXTV_SYSTEM_H_

#define DEMUXCLOCK_MAX	2

struct DemuxClock
{
	INT64	  msPCR;									   /* millisecond */
	INT64	  msSTC;									   /* millisecond */
	INT64	  msPreStc; 								   /* millisecond */
	INT32	  msOffset; 								   /* difference between PCR and STC */
	INT32	  pause;									   /* 1: pause, 0: running */
	pthread_mutex_t    lockPcr;
};

typedef struct {
	pfnDxB_DEMUX_Notify	gpfnSectionNotify;
	pfnDxB_DEMUX_AllocBuffer gpfnSectionAllocBuffer;

	pfnDxB_DEMUX_PES_Notify gpfnSubtitleNotify;
	pfnDxB_DEMUX_AllocBuffer gpfnSubtitleAllocBuffer;
	pfnDxB_DEMUX_FreeBufferForError gpfnSubtitleFreeBuffer;

	pfnDxB_DEMUX_PES_Notify gpfnTeletextNotify;
	pfnDxB_DEMUX_AllocBuffer gpfnTeletextAllocBuffer;
	pfnDxB_DEMUX_FreeBufferForError gpfnTeletextFreeBuffer;

	pfnDxB_DEMUX_PES_Notify gpfnUserDefinedNotify;
	pfnDxB_DEMUX_AllocBuffer gpfnUserDefinedAllocBuffer;
	pfnDxB_DEMUX_FreeBufferForError gpfnUserDefinedFreeBuffer;

	unsigned int guiPCRPid[2];

	unsigned int g_ISDBT_Feature;
	unsigned int g_Parent_Lock;
	int g_LastInputtedPTS;
	pthread_mutex_t    TCCSTCLock;
	int giSTCDelayOffset[2];
	struct DemuxClock demux_clock[DEMUXCLOCK_MAX];
} DemuxSystem, *HandleDemuxSystem;

int TCCDEMUX_SetISDBTFeature(HandleDemuxSystem pHandle, unsigned int feature);
int TCCDEMUX_GetISDBTFeature(HandleDemuxSystem pHandle, unsigned int *feature);
int TCCDEMUX_SetParentLock(HandleDemuxSystem pHandle, unsigned int lock);
int TCCDemux_GetBrazilParentLock(HandleDemuxSystem pHandle);
MPEGDEMUX_ERROR TCCDemux_ResetSystemTime(HandleDemuxSystem pHandle);
INT64 TCCDemux_GetSystemTime(HandleDemuxSystem pHandle, int index);
MPEGDEMUX_ERROR TCCDemux_UpdatePCR (HandleDemuxSystem pHandle, INT32 itype, int index, INT64 pcr);
long long TCCDEMUX_GetSTC(HandleDemuxSystem pHandle, HandleDemux hDMX, int itype, long long lpts, int index, int bPlayMode, int log);
int TCCDEMUX_CBSection(HandleDemuxFilter slot, unsigned char *buf, int size, unsigned long long pts, int done, void *appData);
int TCCDEMUX_CBSubtitlePES(HandleDemuxFilter slot, unsigned char *buf, int size, unsigned long long pts, int done, void *appData);
int TCCDEMUX_CBTeletextPES (HandleDemuxFilter slot, unsigned char *buf, int size, unsigned long long pts, int done, void *appData);
int TCCDEMUX_CBUserDefinedPES(HandleDemuxFilter slot, unsigned char *buf, int size, unsigned long long pts, int done, void *appData);
int TCCDEMUX_Init(HandleDemuxSystem pHandle);
int TCCDEMUX_DeInit(HandleDemuxSystem pHandle);
int TCCDEMUX_ReInit(HandleDemuxSystem pHandle);
void TCCDEMUX_SetSTCOffset(HandleDemuxSystem pHandle, int iType, int iIndex, int iOffset, int iOption);

//For seamless switching
void TCCDEMUX_Compensation_STC(int delay);
int TCCDEMUX_Compensation_get_Excute(void);
int TCCDEMUX_Compensation_Enable(void);
unsigned long long TCCDEMUX_Compensation_get_STC_Delay_Time(void);
#endif
