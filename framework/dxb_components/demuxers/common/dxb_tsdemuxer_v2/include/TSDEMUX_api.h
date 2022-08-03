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

 ****************************************************************************/
#ifndef _TSDEMUX_API_H_
#define _TSDEMUX_API_H_


typedef enum
{
    TCC_TSDEMUX_APP,
    TCC_TSDEMUX_KERNEL //This is valid for tcc88xx upper.
}TCC_TSDEMUX_MODE;


typedef enum
{
	DEMUXPES_VIDEO,
	DEMUXPES_AUDIO,
	DEMUXPES_SUBTITLE,	
	DEMUXPES_TELETEXT,
	DEMUXPES_OTHER,
	DEMUXPES_SUBTITLE_ES, //It exclude PES Header, Only send playload data
	DEMUXPES_TELETEXT_ES, //It exclude PES Header, Only send playload data
	DEMUXPES_OTHER_ES	  //It exclude PES Header, Only send playload data 
} MPEGDEMUX_PES_TYPE;

typedef enum
{
	DEMUXTS_SCRAMBLING,
	DEMUXTS_NOTSCRAMBLING,
	DEMUXTS_NOTDEFINED
}MPEGDEMUX_TS_STATUS;

#ifdef WIN32
typedef __int64 _INT64;
#else
typedef long long _INT64;
#endif

typedef void *HandleDemuxFilter;
typedef void *HandleSectionFilterPara;
typedef int (*FILTERPUSHCALLBACK) (HandleDemuxFilter slot, unsigned char *buf, int size, _INT64 pts, int done, void *appData);
typedef int	(*SETCLEAR_PIDCALLBACK)(int iPID);
typedef int (*SCRAMBLE_NOTIFY_CALLBACK)(unsigned short pid, unsigned char scrambled_state);
typedef int	(*CAS_DECRYPTCALLBACK)(int type, unsigned char *pBuf, int uiSize);

int TSDEMUXApi_GetMode(void);
int TSDEMUXApi_Init(void *arg);
int TSDEMUXApi_DeInit(void);
int TSDEMUXApi_SetPIDFilterFunction(SETCLEAR_PIDCALLBACK pfnSetPID, SETCLEAR_PIDCALLBACK pfnClearPID);
int TSDEMUXApi_SetScrambledNotifyFunction(SCRAMBLE_NOTIFY_CALLBACK pfnScrambledNotify);
int TSDEMUXApi_SetCasDecryptFunction(CAS_DECRYPTCALLBACK pfnCasDecryptCallback );
HandleSectionFilterPara TSDEMUXApi_MakeSectionFilterPara(char *pComp, char *pMask, char *pPole, unsigned int uiSize);
int TSDEMUXApi_DeleteSectionFilterPara(HandleSectionFilterPara pSecFlt);
HandleDemuxFilter TSDEMUXApi_CreateSectionFilter (unsigned int uiPid,int iID,int iRepeat, int iCheckCRC, FILTERPUSHCALLBACK callback, void *arg, HandleSectionFilterPara secflt);
HandleDemuxFilter TSDEMUXApi_CreatePESFilter (unsigned int uiPid,int iID, MPEGDEMUX_PES_TYPE iType,  void *arg, FILTERPUSHCALLBACK callback);
int TSDEMUXApi_GetID(HandleDemuxFilter slothandle, int *piID);
int TSDEMUXApi_GetPID(HandleDemuxFilter slothandle, int *piID);
int TSDEMUXApi_GetUsedSlotNumber(void);
int TSDEMUXApi_DestroyFilter(HandleDemuxFilter slothandle);
int TSDEMUXApi_DestroyFilterByID(int iID);
int TSDEMUXApi_DestroyAllFilter(void);
int TSDEMUXApi_EnableFilter(HandleDemuxFilter slothandle);
int TSDEMUXApi_DisableFilter(HandleDemuxFilter slothandle);
int TSDEMUXApi_ClearAllFilter(void);
int TSDEMUXApi_EnableTSBrokenDataPass(HandleDemuxFilter slothandle);
_INT64 TSDEMUXApi_GetPCR(void);
_INT64 TSDEMUXApi_GetSTC(void);
int TSDEMUXApi_ResetSTC(void);
int TSDEMUXApi_PauseSTC(void);
int TSDEMUXApi_ResumeSTC(void);
int TSDEMUXApi_UpdatePCR(void);
int TSDEMUXApi_SetPcrPID(int pid);
int TSDEMUXApi_GetPcrPID(int *ppcrpid);
int TSDEMUXApi_ClearPcrPID(void);
int TSDEMUXApi_BufferReAlloc(HandleDemuxFilter slothandle, unsigned int uiBufferSize);
int TSDEMUXApi_ResetBuffer(HandleDemuxFilter slothandle);
int TSDEMUXApi_InitBuffer(HandleDemuxFilter slothandle);
int TSDEMUXApi_ChangeESBufferSize(unsigned int uiVideoBufSize, unsigned int uiVideoESMaxSize, unsigned int uiAudioBufSize, unsigned int uiAudioESMaxSize);
int TSDEMUXApi_SetStaticBuffer(HandleDemuxFilter slothandle, unsigned char *pucStaticBuffer, unsigned int uiBufferSize);
int TCC_MPEGDEMUXER(unsigned char *pInput, unsigned int size );
int TSDEMUXApi_GetBufferSize(HandleDemuxFilter slothandle, int *pBufferSize);
int TSDEMUXApi_UpdateWriteBuffer(HandleDemuxFilter slothandle, unsigned char *pucReadPtr, unsigned char *pucWritePtr, unsigned int uiWriteSize);
int TSDEMUXApi_GetFreeBufferSize(HandleDemuxFilter slothandle, unsigned char *pucReadPtr, unsigned char *pucWritePtr, int *piFreeBufferSize);

int TSDEMUXApi_GetStreamType(HandleDemuxFilter slothandle);
int TSDEMUXApi_SetStreamType(HandleDemuxFilter slothandle, int iType, int ParserOnOff);
int TSDEMUXApi_EnableStreamParser(HandleDemuxFilter slothandle);
int TSDEMUXApi_DisableStreamParser(HandleDemuxFilter slothandle);
int TSDEMUXApi_ResetStreamParser(HandleDemuxFilter slothandle);

int TSDEMUXApi_StartTSMornitor(int iPID, unsigned int *puiHandle);
int TSDEMUXApi_StopTSMornitor(unsigned int uiHandle);
int TSDEMUXApi_GetTSMornitor(unsigned int uiHandle, MPEGDEMUX_TS_STATUS *peTSStatus);
unsigned int TSDEMUXApi_GetTCCTime(void); //return msec


//Below is only valid on TCC_TSDEMUX_KERNEL
int TSDEMUXApiK_WriteTSData(unsigned char *p, int size);
int TSDEMUXApiK_GetPushed(HandleDemuxFilter slothandle);
int TSDEMUXApiK_GetSlotData(HandleDemuxFilter slothandle, unsigned char *pbuffer, unsigned int *puisize, unsigned int *puipts_ms);

int TSDEMUXApi_GetPTSDTSFlag(HandleDemuxFilter slothandle, int *pData);
int TSDEMUXApi_GetStreamId(HandleDemuxFilter slothandle, int *pData);
	
#endif
