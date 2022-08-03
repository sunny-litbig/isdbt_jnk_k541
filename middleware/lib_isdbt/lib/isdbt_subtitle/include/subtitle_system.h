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


#ifndef __SUBTITLE_SYSTEM_H__
#define __SUBTITLE_SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ISDBT_Caption.h>
#include <subtitle_main.h>

/****************************************************************************
DEFINITION OF STRUCTURE
****************************************************************************/

extern void subtitle_system_set_stc_index(int index);
extern int	subtitle_system_get_stc_index(void);
extern unsigned long long subtitle_system_get_systime(void);
extern int subtitle_system_check_noupdate(unsigned long long cur_pts);
extern int subtitle_system_get_disp_info(void *p);
extern int subtitle_system_get_output_disp_info(void *p);
extern int subtitle_system_init(SUB_SYS_INFO_TYPE *p_sys_info, int seg_type, int country, int fb_type, int raw_w, int raw_h, int view_w, int view_h);
extern long long subtitle_system_get_delay(unsigned long long cur_pts);
extern void subtitle_set_res_changed(int flag);
extern int subtitle_get_res_changed(void);
extern unsigned long long subtitle_system_get_stc_delay_time(void);
extern int subtitle_system_get_stc_delay_excute(void);
#ifdef __cplusplus
}
#endif

#endif	/* __SUBTITLE_SYSTEM_H__ */

