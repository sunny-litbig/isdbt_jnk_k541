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

#ifndef	_TCC_ISDBT_MANAGER_AUDIO_H_
#define	_TCC_ISDBT_MANAGER_AUDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

int tcc_manager_audio_init(void);
int tcc_manager_audio_deinit(void);
int tcc_manager_audio_start(unsigned int uidevid, unsigned int ulAudioFormat);
int tcc_manager_audio_stop(unsigned int uidevid);
int tcc_manager_audio_alsa_close(unsigned int uidevid, unsigned int uiStop);
int tcc_manager_audio_alsa_close_flag(unsigned int uiClose);
int tcc_manager_audio_stereo(unsigned int uidevid, unsigned int ulMode);
int tcc_manager_audio_volume(unsigned int uidevid, unsigned int ulVolume);
int tcc_manager_audio_mute(unsigned int uidevid, unsigned int bOnOff);
int tcc_manager_audio_select_output(unsigned int uidevid, unsigned int isEnableAudioOutput);
int tcc_manager_audio_set_dualmono (unsigned int uidevid, unsigned int audio_mode);
int tcc_manager_audio_issupport_country(unsigned int uidevid, unsigned int uiCountry);
int tcc_manager_audio_get_audiotype(int devid, int *piNumCh, int *piAudioMode);
extern int tcc_manager_audio_serviceID_disable_output(int check_flag);
int tcc_manager_audio_set_AudioStartSyncWithVideo(unsigned int uiOnOff);
int tcc_manager_audio_setframedropflag(unsigned int check_flag);
int tcc_manager_audio_setSeamlessSwitchCompensation(int iOnOff, int iInterval, int iStrength, int iNtimes, int iRange, int iGapadjust, int iGapadjust2, int iMuliplier);
int tcc_manager_audio_set_proprietarydata (unsigned int channel_index, unsigned int service_id, unsigned int sub_service_id, int dual_mode, int supportPrimary);
int tcc_manager_audio_get_SeamlessValue(int *state, int *cval, int *pval);

#ifdef __cplusplus
}
#endif
#endif


