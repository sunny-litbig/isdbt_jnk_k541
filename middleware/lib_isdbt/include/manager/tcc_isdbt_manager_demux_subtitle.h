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
#ifndef __TCC_ISDBT_MANAGER_DEMUX_SUBTITLE_H__
#define __TCC_ISDBT_MANAGER_DEMUX_SUBTITLE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tcc_dxb_interface_type.h"
#include "tcc_dxb_interface_demux.h"

extern DxB_ERR_CODE tcc_demux_subtitle_notify(UINT8 *pucBuf, UINT32 uiBufSize, UINT64 ullPTS, UINT32 ulRequestId, void *pUserData);
extern DxB_ERR_CODE tcc_demux_superimpose_notify(UINT8 *pucBuf, UINT32 uiBufSize, UINT64 ullPTS, UINT32 ulRequestId, void *pUserData);
extern void* tcc_demux_subtitle_decoder(void *arg);
extern int tcc_manager_demux_subtitle_init(void);
extern int tcc_manager_demux_subtitle_deinit(void);
extern int tcc_manager_demux_subtitle_stop(int init);
extern int tcc_manager_demux_subtitle_play(int iStPID, int iSiPID, int iSegType, int iCountryCode, int iRaw_w, int iRaw_h, int iView_w, int iView_h, int init);
	
#ifdef __cplusplus
}
#endif

#endif 	//__TCC_ISDBT_MANAGER_DEMUX_SUBTITLE_H__