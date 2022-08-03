/****************************************************************************

Copyright (C) 2013 Telechips Inc.


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions 
andlimitations under the License.

****************************************************************************/

/****************************************************************************

Revision History

****************************************************************************

****************************************************************************/
#ifndef	_TCC_DXB_INTERFACE_REMOTEPLAY_H_
#define	_TCC_DXB_INTERFACE_REMOTEPLAY_H_
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
* declarations
******************************************************************************/
DxB_ERR_CODE TCC_DxB_REMOTEPLAY_Open(void);
DxB_ERR_CODE TCC_DxB_REMOTEPLAY_Close(void);
DxB_ERR_CODE TCC_DxB_REMOTEPLAY_SetVideoInfo(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiFrameRate);
DxB_ERR_CODE TCC_DxB_REMOTEPLAY_WriteVideo(unsigned char *pucData, unsigned int uiSize, unsigned int uiPTS, unsigned int uiPicType);
DxB_ERR_CODE TCC_DxB_REMOTEPLAY_WriteAudio(unsigned char *pucData, unsigned int uiSize, unsigned int uiPTS);
DxB_ERR_CODE TCC_DxB_REMOTEPLAY_SetAudioInfo(unsigned int uiSampleRate, unsigned int uiChannelCounts);
DxB_ERR_CODE TCC_DxB_REMOTEPLAY_WriteSubtitle(void *pData, unsigned int uiFormat);
#ifdef __cplusplus
}
#endif    
#endif


