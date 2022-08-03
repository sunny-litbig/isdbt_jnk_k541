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


#ifndef __SUBTITLE_PARSER_H__
#define __SUBTITLE_PARSER_H__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
DEFINITION OF STRUCTURE
****************************************************************************/
#include <ISDBT_Caption.h>

extern int ISDBTCC_Check_DataGroupData(T_SUB_CONTEXT *p_sub_ctx, TS_PES_CAP_DATAGROUP *pDataGroup);
extern void ISDBTCC_Handle_CaptionManagementData(int data_type, TS_PES_CAP_DATAGROUP *pDataGroup);
extern void ISDBTCC_Handle_CaptionStatementData(int data_type, TS_PES_CAP_DATAGROUP *pDataGroup);

#ifdef __cplusplus
}
#endif

#endif	/* __SUBTITLE_PARSER_H__ */

