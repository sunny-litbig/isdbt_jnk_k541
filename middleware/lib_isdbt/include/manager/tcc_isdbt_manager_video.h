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

#ifndef	_TCC_ISDBT_MANAGER_VIDEO_H_
#define	_TCC_ISDBT_MANAGER_VIDEO_H_

#ifdef __cplusplus
extern "C" {
#endif

int tcc_manager_video_init(void);
int tcc_manager_video_deinit(void);
int tcc_manager_video_start(unsigned int uidevid, unsigned int ulVideoFormat);
int tcc_manager_video_stop(unsigned int uidevid);
int tcc_manager_video_pause(unsigned int uidevid, unsigned int uiOnOff);
int tcc_manager_video_initSurface(int arg);
int tcc_manager_video_deinitSurface();
int tcc_manager_video_setSurface(void *nativeWidow);
int tcc_manager_video_useSurface(void);
int tcc_manager_video_releaseSurface(void);
int tcc_manager_video_capture(unsigned int uidevid,unsigned char *pucFileName);
int tcc_manager_video_select_display_output(unsigned int uidevid);
int tcc_manager_video_issupport_country(unsigned int uidevid, unsigned int uiCountry);
int tcc_manager_video_enable_display(unsigned int uidevid, unsigned int uiOnOff);
int tcc_manager_video_refreshDisplay(unsigned int uidevid);
int tcc_manager_video_set_proprietarydata (unsigned int channel_index);
extern int tcc_manager_video_serviceID_disable_display(int check_flag);
int tcc_manager_video_setfirstframebypass(unsigned int uiOnOff);
int tcc_manager_video_setfirstframeafterseek(unsigned int uiOnOff);
int tcc_manager_video_setframedropflag(unsigned int check_flag);
int tcc_manager_video_getdisplayedfirstframe(void);
#ifdef __cplusplus
}
#endif

#endif
