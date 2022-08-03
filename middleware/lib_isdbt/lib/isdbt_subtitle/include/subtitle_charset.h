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


#ifndef __SUBTITLE_CHARSET_H__
#define __SUBTITLE_CHARSET_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ISDBT_Caption.h"

extern void ISDBTCC_Handle_KanjiCharSet(T_SUB_CONTEXT *p_sub_ctx, unsigned char *pCharCode);
extern void ISDBTCC_Handle_AlphanumericCharSet(T_SUB_CONTEXT *p_sub_ctx, unsigned char *pCharCode);
extern void ISDBTCC_Handle_LatinExtensionCharSet(T_SUB_CONTEXT *p_sub_ctx, unsigned char *pCharCode);
extern void ISDBTCC_Handle_SpecialCharSet(T_SUB_CONTEXT *p_sub_ctx, unsigned char *pCharCode);
extern void ISDBTCC_Handle_HiraganaCharSet(T_SUB_CONTEXT *p_sub_ctx, unsigned char *pCharCode);
extern void ISDBTCC_Handle_KataganaCharSet(T_SUB_CONTEXT *p_sub_ctx, unsigned char *pCharCode);
extern void ISDBTCC_Handle_DRCSCharSet(T_SUB_CONTEXT *p_sub_ctx, unsigned char *pCharCode, unsigned int iCharSet);
extern void ISDBTCC_Handle_CaptionManagementData(int data_type, TS_PES_CAP_DATAGROUP *pDataGroup);

extern int ISDBTCap_ControlCharDisp(T_SUB_CONTEXT *p_sub_ctx, int mngr, unsigned int ucControlType, unsigned int param1, unsigned int param2, unsigned int param3);

#ifdef __cplusplus
}
#endif

#endif	/* __SUBTITLE_CHARSET_H__ */

