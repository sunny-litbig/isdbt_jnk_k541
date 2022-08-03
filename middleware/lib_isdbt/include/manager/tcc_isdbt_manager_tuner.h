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

#ifndef	_TCC_ISDBT_MANAGER_TUNER_H_
#define	_TCC_ISDBT_MANAGER_TUNER_H_

#ifdef __cplusplus
extern "C" {
#endif

int tcc_manager_tuner_init(int uiBBSelect);
int tcc_manager_tuner_deinit(void);
int tcc_manager_tuner_open(int icountrycode);
int tcc_manager_tuner_close(void);
int tcc_manager_tuner_is_suspending(void);
int tcc_manager_tuner_scanflag_init(void);
int tcc_manager_tuner_scanflag_get (void);
int tcc_manager_tuner_informal_scan(int country_code, int channel_num, int tsid, int options);
int tcc_manager_tuner_scan(int scan_type, int country_code, int region_code, int channel_num, int options);
int tcc_manager_tuner_scan_manual (int channel_num, int country_code, int done_flag);
void tcc_manager_tuner_scan_cancel_notify (void);
int tcc_manager_tuner_set_channel(int ich);
int tcc_manager_tuner_get_strength(int *sqinfo);
int tcc_manager_tuner_scan_cancel(void);
int tcc_manager_tuner_register_pid(int pid);
int tcc_manager_tuner_unregister_pid(int pid);
int tcc_manager_tuner_set_area (unsigned int uiArea);
int tcc_manager_tuner_get_ews_flag(void *pStartFlag);
int tcc_manager_tuner_handover_load(int iChannelNumber, int iServiceID);
int tcc_manager_tuner_handover_update(int iNetworkID);
int tcc_manager_tuner_handover_clear(void);
int tcc_manager_tuner_handover_scan(int channel_num, int country_code, int search_affiliation, int *same_channel);
int tcc_manager_tuner_cas_open(unsigned char _casRound, unsigned char * _systemKey);
int tcc_manager_tuner_cas_key_multi2(unsigned char _parity, unsigned char *_key, unsigned char _keyLength, unsigned char *_initVector,unsigned char _initVectorLength);
int tcc_manager_tuner_cas_set_pid(unsigned int *_pids, unsigned int _numberOfPids);
int tcc_manager_tuner_set_FreqBand(int freq_band);
int tcc_manager_tuner_reset(int countrycode, int ich);
int tcc_manager_tuner_set_CustomTuner(int size, void *arg);
int tcc_manager_tuner_get_CustomTuner(int *size, void *arg);
int tcc_manager_tuner_UserLoopStopCmd(int moduleIndex);
int tcc_manager_tuner_get_lastchannel(void);

#ifdef __cplusplus
}
#endif

#endif

