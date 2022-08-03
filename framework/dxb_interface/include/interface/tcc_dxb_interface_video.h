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

#ifndef _TCC_DXB_INTERFACE_VIDEO_H_
#define _TCC_DXB_INTERFACE_VIDEO_H_
#include "tcc_dxb_interface_type.h"

/**
 * Define video's notifying event
 */
typedef	enum
{
    DxB_VIDEO_NOTIFY_FIRSTFRAME_DISPLAYED,
    DxB_VIDEO_NOTIFY_CAPTURE_DONE,
    DxB_VIDEO_NOTIFY_USER_DATA_AVAILABLE,
    DxB_VIDEO_NOTIFY_VIDEO_DEFINITION_UPDATE,
    DxB_VIDEO_NOTIFY_VIDEO_ERROR,
	DxB_VIDEO_NOTIFY_EVT_END
} DxB_VIDEO_NOTIFY_EVT;

typedef void (*pfnDxB_VIDEO_EVENT_Notify)(DxB_VIDEO_NOTIFY_EVT nEvent, void *pCallbackData, void *pUserData);


/********************************************************************************************/
/********************************************************************************************
						FOR MW LAYER FUNCTION
********************************************************************************************/
/********************************************************************************************/
DxB_ERR_CODE TCC_DxB_VIDEO_Init(DxBInterface *hInterface);
DxB_ERR_CODE TCC_DxB_VIDEO_Deinit(DxBInterface *hInterface);
DxB_ERR_CODE TCC_DxB_VIDEO_Start(DxBInterface *hInterface, UINT32 ulDevId, UINT32 ulVideoFormat);
DxB_ERR_CODE TCC_DxB_VIDEO_Stop(DxBInterface *hInterface, UINT32 ulDevId);
DxB_ERR_CODE TCC_DxB_VIDEO_Pause(DxBInterface *hInterface, UINT32 ulDevId, BOOLEAN bOn);
DxB_ERR_CODE TCC_DxB_VIDEO_Flush(DxBInterface *hInterface);
DxB_ERR_CODE TCC_DxB_VIDEO_StartCapture(DxBInterface *hInterface, UINT32 ulDevId, UINT16 usTargetWidth, UINT16 usTargetHeight, UINT8 *pucFileName);

DxB_ERR_CODE TCC_DxB_VIDEO_SelectDisplayOutput(DxBInterface *hInterface, UINT32 ulDevId);
DxB_ERR_CODE TCC_DxB_VIDEO_RefreshDisplay(DxBInterface *hInterface, UINT32 ulDevId);
DxB_ERR_CODE TCC_DxB_VIDEO_IsSupportCountry(DxBInterface *hInterface, UINT32 ulDevId, UINT32 ulCountry);
DxB_ERR_CODE TCC_DxB_VIDEO_EnableDisplay(DxBInterface *hInterface, UINT32 ulDevId);
DxB_ERR_CODE TCC_DxB_VIDEO_DisableDisplay(DxBInterface *hInterface, UINT32 ulDevId);
DxB_ERR_CODE TCC_DxB_VIDEO_LCDUpdate(DxBInterface *hInterface, UINT32 ulContentsType);
DxB_ERR_CODE TCC_DxB_VIDEO_GetVideoInfo(DxBInterface *hInterface, UINT32 ulDevId, UINT32 *videoWidth, UINT32 *videoHeight);
DxB_ERR_CODE TCC_DxB_VIDEO_RegisterEventCallback(DxBInterface *hInterface, pfnDxB_VIDEO_EVENT_Notify pfnEventCallback, void *pUserData);
DxB_ERR_CODE TCC_DxB_VIDEO_SetPause(DxBInterface *hInterface, UINT32 ulDevId, UINT32 pause);
DxB_ERR_CODE TCC_DxB_VIDEO_IFrameSearchEnable (DxBInterface *hInterface, UINT32 ulDevId);
DxB_ERR_CODE TCC_DxB_VIDEO_SetSinkByPass(DxBInterface *hInterface, UINT32 ulDevId, UINT32 sink);
DxB_ERR_CODE TCC_DxB_VIDEO_SendEvent(DxBInterface *hInterface, DxB_VIDEO_NOTIFY_EVT nEvent, void *pCallbackData);
DxB_ERR_CODE TCC_DxB_VIDEO_InitSurface(DxBInterface *hInterface, UINT32 ulDevId, int arg);
DxB_ERR_CODE TCC_DxB_VIDEO_DeinitSurface(DxBInterface *hInterface, UINT32 ulDevId);
DxB_ERR_CODE TCC_DxB_VIDEO_SetSurface(DxBInterface *hInterface, UINT32 ulDevId, void *nativeWidow);
DxB_ERR_CODE TCC_DxB_VIDEO_UseSurface(DxBInterface *hInterface);
DxB_ERR_CODE TCC_DxB_VIDEO_ReleaseSurface(DxBInterface *hInterface);
DxB_ERR_CODE TCC_DxB_VIDEO_Use(DxBInterface *hInterface, INT32 arg);
DxB_ERR_CODE TCC_DxB_VIDEO_Release(DxBInterface *hInterface);
DxB_ERR_CODE TCC_DxB_VIDEO_Subtitle(DxBInterface *hInterface, void *arg);
DxB_ERR_CODE TCC_DxB_VIDEO_SupportFieldDecoding(DxBInterface *hInterface, UINT32 OnOff);
DxB_ERR_CODE TCC_DxB_VIDEO_SupportIFrameSearch(DxBInterface *hInterface, UINT32 OnOff);
DxB_ERR_CODE TCC_DxB_VIDEO_SupportUsingErrorMB(DxBInterface *hInterface, UINT32 OnOff);
DxB_ERR_CODE TCC_DxB_VIDEO_SupportDirectDisplay(DxBInterface *hInterface, UINT32 OnOff);
unsigned int TCC_DxB_VIDEO_GetDisplayFlag(DxBInterface *hInterface);
DxB_ERR_CODE TCC_DxB_VIDEO_SetProprietaryData (DxBInterface *hInterface, UINT32 channel_index);
DxB_ERR_CODE TCC_DxB_VIDEO_SetVpuReset(DxBInterface *hInterface, UINT32 ulDevId);
DxB_ERR_CODE TCC_DxB_VIDEO_SetActiveMode(DxBInterface *hInterface, UINT32 ulDevId, UINT32 activemode);
DxB_ERR_CODE TCC_DxB_VIDEO_ServiceIDDisableDisplay(DxBInterface *hInterface, UINT32 check_flag);
DxB_ERR_CODE TCC_DxB_VIDEO_GetDisplayedFirstFrame(DxBInterface *hInterface, UINT32 ulDevId, INT32 *displayedfirstframe);
DxB_ERR_CODE TCC_DxB_VIDEO_SetFirstFrameByPass(DxBInterface *hInterface, UINT32 OnOff);
DxB_ERR_CODE TCC_DxB_VIDEO_SetFirstFrameAfterSeek(DxBInterface *hInterface, UINT32 OnOff);
DxB_ERR_CODE TCC_DxB_VIDEO_SetFrameDropFlag(DxBInterface *hInterface, UINT32 check_flag);
DxB_ERR_CODE TCC_DxB_VIDEO_SetSTCFunction(DxBInterface *hInterface, void *pfnGetSTCFunc, void *pvApp);
DxB_ERR_CODE TCC_DxB_VIDEO_CtrlLastFrame(DxBInterface *hInterface, UINT32 OnOff);
DxB_ERR_CODE TCC_DxB_VIDEO_SetDisplayPosition(DxBInterface *hInterface, INT32 iPosX, INT32 iPosY, INT32 iWidth, INT32 iHeight);
#endif

