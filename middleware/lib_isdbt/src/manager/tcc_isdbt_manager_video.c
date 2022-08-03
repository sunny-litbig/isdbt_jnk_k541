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

#define LOG_NDEBUG 0
#define LOG_TAG	"ISDBT_MANAGER_VIDEO"
#include <utils/Log.h>

#include "tcc_isdbt_event.h"
#include "tcc_dxb_interface_video.h"
#include "tcc_isdbt_manager_video.h"

extern DxBInterface *hInterface;

void tcc_video_event(DxB_VIDEO_NOTIFY_EVT nEvent, void *pEventData, void *pUserData)
{
//    ALOGD("%s:[%d][%p]", __func__, nEvent, pEventData);
    switch(nEvent)
    {
        case DxB_VIDEO_NOTIFY_FIRSTFRAME_DISPLAYED:
            TCCDxBEvent_FirstFrameDisplayed(pEventData);
            break;
        case DxB_VIDEO_NOTIFY_VIDEO_DEFINITION_UPDATE:
            TCCDxBEvent_VideoDefinitionUpdate(pEventData);
            break;
        case DxB_VIDEO_NOTIFY_VIDEO_ERROR:
			TCCDxBEvent_VideoError(pEventData);
			break;
        default:
            break;
    }
}

int tcc_manager_video_init(void)
{
    TCC_DxB_VIDEO_Init(hInterface);
    TCC_DxB_VIDEO_RegisterEventCallback(hInterface, tcc_video_event, 0);
	return 0;
}

int tcc_manager_video_deinit(void)
{
    TCC_DxB_VIDEO_Deinit(hInterface);
	return 0;
}

int tcc_manager_video_start(unsigned int uidevid, unsigned int ulVideoFormat)
{
	return TCC_DxB_VIDEO_Start(hInterface, uidevid, ulVideoFormat);
}

int tcc_manager_video_stop(unsigned int uidevid)
{
	return TCC_DxB_VIDEO_Stop(hInterface, uidevid);
}

int tcc_manager_video_pause(unsigned int uidevid, unsigned int uiOnOff)
{
	return 0;
}

int tcc_manager_video_initSurface(int arg)
{
    return TCC_DxB_VIDEO_InitSurface(hInterface, 0, arg);
}

int tcc_manager_video_deinitSurface()
{
    return TCC_DxB_VIDEO_DeinitSurface(hInterface, 0);
}

int tcc_manager_video_setSurface(void *nativeWidow)
{
    return TCC_DxB_VIDEO_SetSurface(hInterface, 0, nativeWidow);
}

int tcc_manager_video_useSurface(void)
{
    return TCC_DxB_VIDEO_UseSurface(hInterface);
}

int tcc_manager_video_releaseSurface(void)
{
    return TCC_DxB_VIDEO_ReleaseSurface(hInterface);
}

int tcc_manager_video_capture(unsigned int uidevid, unsigned char *pucFileName)
{
    TCC_DxB_VIDEO_StartCapture(hInterface, uidevid, 0, 0, pucFileName);
    return 0;
}

int tcc_manager_video_select_display_output(unsigned int uidevid)
{
	return TCC_DxB_VIDEO_SelectDisplayOutput(hInterface, uidevid);
}

int tcc_manager_video_issupport_country(unsigned int uidevid, unsigned int uiCountry)
{
	return TCC_DxB_VIDEO_IsSupportCountry(hInterface, uidevid, uiCountry);
}

int tcc_manager_video_enable_display(unsigned int uidevid, unsigned int uiOnOff)
{
    if(uiOnOff)
        return TCC_DxB_VIDEO_EnableDisplay(hInterface, uidevid);
    else
        return TCC_DxB_VIDEO_DisableDisplay(hInterface, uidevid);
}

int tcc_manager_video_refreshDisplay(unsigned int uidevid)
{
    return TCC_DxB_VIDEO_RefreshDisplay(hInterface, uidevid);
}

int tcc_manager_video_set_proprietarydata (unsigned int channel_index)
{
	return TCC_DxB_VIDEO_SetProprietaryData(hInterface, channel_index);
}

int tcc_manager_video_serviceID_disable_display(int check_flag)
{
	//ALOGE("[#]In %s (%d)\n", __func__, check_flag);
	return TCC_DxB_VIDEO_ServiceIDDisableDisplay(hInterface, check_flag);
}

int tcc_manager_video_getdisplayedfirstframe(void)
{
	int iDisplayedFirstFrame[2] = {0, };
	int iRet = 0;

	iRet |= TCC_DxB_VIDEO_GetDisplayedFirstFrame(hInterface, 0, &iDisplayedFirstFrame[0]);
	iRet |= TCC_DxB_VIDEO_GetDisplayedFirstFrame(hInterface, 1, &iDisplayedFirstFrame[1]);
	if(iRet) {
		ALOGE("%s:%d error[%d]", __func__, __LINE__, iRet);
		return 0;
	}
	else {
		if(iDisplayedFirstFrame[0] || iDisplayedFirstFrame[1])
			return 1;
		return 0;
	}
}

int tcc_manager_video_setfirstframebypass(unsigned int uiOnOff)
{
	return TCC_DxB_VIDEO_SetFirstFrameByPass(hInterface, uiOnOff);
}

int tcc_manager_video_setfirstframeafterseek(unsigned int uiOnOff)
{
	return TCC_DxB_VIDEO_SetFirstFrameAfterSeek(hInterface, uiOnOff);
}

int tcc_manager_video_setframedropflag(unsigned int check_flag)
{
	return TCC_DxB_VIDEO_SetFrameDropFlag(hInterface, check_flag);
}

