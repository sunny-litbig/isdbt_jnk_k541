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
#define LOG_TAG	"ISDBT_MANAGER_AUDIO"
#include <utils/Log.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "tcc_dxb_interface_audio.h" 
#include "tcc_isdbt_manager_audio.h"

extern DxBInterface *hInterface;

void tcc_audio_event(DxB_AUDIO_NOTIFY_EVT nEvent, void *pEventData, void *pUserData)
{
    ALOGD("%s:[%d][%p]", __func__, nEvent, pEventData);
}

int tcc_manager_audio_init(void)
{
	TCC_DxB_AUDIO_Init(hInterface);
	TCC_DxB_AUDIO_RegisterEventCallback(hInterface, tcc_audio_event, 0);
	return 0;
}

int tcc_manager_audio_deinit(void)
{
	TCC_DxB_AUDIO_Deinit(hInterface);
	return 0;
}

int tcc_manager_audio_start(unsigned int uidevid, unsigned int ulAudioFormat)
{
	return TCC_DxB_AUDIO_Start(hInterface, uidevid, ulAudioFormat);
}

int tcc_manager_audio_stop(unsigned int uidevid)
{
	return TCC_DxB_AUDIO_Stop(hInterface, uidevid);
}

int tcc_manager_audio_alsa_close(unsigned int uidevid, unsigned int uiStop)
{
	return TCC_DxB_AUDIO_StopRequest(hInterface, uidevid, uiStop);
}

int tcc_manager_audio_alsa_close_flag(unsigned int uiClose)
{
	return TCC_DxB_AUDIO_CloseAlsaFlag(hInterface, uiClose);
}

int tcc_manager_audio_stereo(unsigned int uidevid, unsigned int ulMode)
{
	return TCC_DxB_AUDIO_SetStereo(hInterface, uidevid, ulMode);
}

int tcc_manager_audio_volume(unsigned int uidevid, unsigned int ulVolume)
{
	return TCC_DxB_AUDIO_SetVolume(hInterface, uidevid, ulVolume);
}

int tcc_manager_audio_mute(unsigned int uidevid, unsigned int bOnOff)
{
	return TCC_DxB_AUDIO_SetMute(hInterface, uidevid, bOnOff);
}

int tcc_manager_audio_select_output(unsigned int uidevid, unsigned int isEnableAudioOutput)
{
	return TCC_DxB_AUDIO_SelectOutput(hInterface, uidevid, isEnableAudioOutput);
}

int tcc_manager_audio_set_dualmono (unsigned int uidevid, unsigned int audio_mode)
{
	return	TCC_DxB_AUDIO_SetDualMono(hInterface, uidevid, audio_mode);
}

int tcc_manager_audio_issupport_country(unsigned int uidevid, unsigned int uiCountry)
{
	return TCC_DxB_AUDIO_IsSupportCountry(hInterface, uidevid, uiCountry);
}

int tcc_manager_audio_get_audiotype(int devid, int *piNumCh, int *piAudioMode)
{
	return TCC_DxB_AUDIO_GetAudioType (hInterface, devid, piNumCh, piAudioMode);
}

int tcc_manager_audio_serviceID_disable_output(int check_flag)
{
	//ALOGE("[#]In %s (%d)\n", __func__, check_flag);
	return TCC_DxB_AUDIO_ServiceIDDisableOutput(hInterface, check_flag);
}

int tcc_manager_audio_set_AudioStartSyncWithVideo(unsigned int uiOnOff)
{
	return TCC_DxB_AUDIO_SetAudioStartSyncWithVideo(hInterface, uiOnOff);
}

int tcc_manager_audio_setframedropflag(unsigned int check_flag)
{
	return TCC_DxB_AUDIO_SetFrameDropFlag(hInterface, check_flag);
}

int tcc_manager_audio_setSeamlessSwitchCompensation(int iOnOff, int iInterval, int iStrength, int iNtimes, int iRange, int iGapadjust, int iGapadjust2, int iMuliplier)
{
	ALOGE("\033[32m [%s] [%d:%d:%d:%d:%d:%d:%d:%d]\033[m\n", __func__, iOnOff, iInterval, iStrength, iNtimes, iRange, iGapadjust, iGapadjust2, iMuliplier);
	return TCC_DxB_AUDIO_setSeamlessSwitchCompensation(hInterface, iOnOff, iInterval, iStrength, iNtimes, iRange, iGapadjust, iGapadjust2, iMuliplier);
}

int tcc_manager_audio_set_proprietarydata (unsigned int channel_index, unsigned int service_id, unsigned int sub_service_id, int dual_mode, int supportPrimary)
{
	return TCC_DxB_AUDIO_SetProprietaryData(hInterface, channel_index, service_id, sub_service_id, dual_mode, supportPrimary);
}

int tcc_manager_audio_get_SeamlessValue(int *state, int *cval, int *pval)
{
	return TCC_DxB_AUDIO_GetSeamlessValue(hInterface, state, cval, pval);
}
