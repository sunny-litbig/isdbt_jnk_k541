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

/******************************************************************************
* include 
******************************************************************************/

#ifndef _TCC_DEMUX_EVENT_H_
#define	_TCC_DEMUX_EVENT_H_

#include "OMX_Core.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
* defines 
******************************************************************************/
#define 	SIZEOF_VIDEO_ES_BUFFER	(1024*1024*2)


/******************************************************************************
* typedefs & structure
******************************************************************************/

enum {
	ES_VIDEO_MAIN = 0,
	ES_VIDEO_SUB,
	ES_VIDEO_MAX
};

typedef enum
{
	PTSCORRECTION_STATUS_OK		= 0,
	PTSCORRECTION_STATUS_CORRECTION_READY,
	PTSCORRECTION_STATUS_PUT_FAIL,
	PTSCORRECTION_STATUS_NOTREADYYET,
	PTSCORRECTION_STATUS_DATAGETFAIL,
	PTSCORRECTION_STATUS_PTSADJUSTFAIL,
	PTSCORRECTION_STATUS_ESBUFUPDATEFAIL,
	PTSCORRECTION_STATUS_MAX,
}DEMUX_PTSCORRECTION_STATUS_TYPE;

typedef struct
{
	unsigned int	iWritePtr;
	unsigned int	iReadPtr;
	unsigned int	iBasePtr;
	unsigned int	iBufSize;
}TCC_DEMUX_ESBUFFER;

typedef struct
{
	long		lPTSInterval;
	long long	llOrgPTSInterval;
	int			iCount;
	int	 		iTotalCount;

}TCC_PTS_ADJUSTVIDEOPTSINFO;

typedef struct
{
	int 		 	iPTS;
	unsigned int 	uiFrameSize;
	unsigned char *pFrameData;
	long long 	llOrgPTS;
	int 			iRequestID;
}TCC_DEMUX_ESDATA;

typedef void* HandleDemuxMsg;


/******************************************************************************
* globals
******************************************************************************/

/******************************************************************************
* locals
******************************************************************************/

/******************************************************************************
* declarations
******************************************************************************/

HandleDemuxMsg TCCDEMUXMsg_Init(void);
int TCCDEMUXMsg_DeInit(HandleDemuxMsg pHandle);
int TCCDEMUXMsg_ReInit(HandleDemuxMsg pHandle);
int TCCDEMUXMsg_GetCount(HandleDemuxMsg pHandle, int iRequestID);
void TCCDEMUXMsg_UpdatelastPts(HandleDemuxMsg pHandle, int iRequestID, unsigned long long pts);
int TCCDEMUXMsg_CorrectionReadyCheck(HandleDemuxMsg pHandle, int iRequestID, unsigned long long pts);
unsigned int TCCDEMUXMsg_PutEs(HandleDemuxMsg pHandle, int iRequestID, unsigned char *buf, int size, unsigned long long pts);
unsigned int TCCDEMUXMsg_GetEs(HandleDemuxMsg pHandle, int iRequestID, OMX_BUFFERHEADERTYPE* pOutputBuffer, unsigned long long pts);
int TCCDEMUX_UpdateESBuffer(HandleDemuxMsg pHandle, int iRequestID, unsigned char *buf, int size);
int TCCDEMUX_GetAdjustVideoPTS(HandleDemuxMsg pHandle, int iRequestID, TCC_DEMUX_ESDATA *pESData, int *plAdjustVideoPTS, long long *pllAdjustVideoOrgPTS);

#ifdef __cplusplus
}
#endif

#endif
