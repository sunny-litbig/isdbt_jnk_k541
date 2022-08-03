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


#ifndef __SUBTITLE_QUEUE_H__
#define __SUBTITLE_QUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
DEFINITION OF STRUCTURE
****************************************************************************/
extern int subtitle_queue_init(void);
extern void subtitle_queue_exit(void);
extern int subtitle_queue_put(int type, int handle, int x, int y, int w, int h, unsigned long long pts, int flash_handle);
extern int subtitle_queue_put_first(int type, int handle, int x, int y, int w, int h, unsigned long long pts, int flash_handle);
extern int subtitle_queue_get(int type, int *p_handle, int *p_x, int *p_y, int *p_w, int *p_h, unsigned long long *p_pts, int *p_flash_handle);
extern int subtitle_queue_peek(int type, int *p_handle, int *p_x, int *p_y, int *p_w, int *p_h, unsigned long long *p_pts, int *p_flash_handle);
extern int subtitle_queue_remove_all(int type);
extern int subtitle_queue_put_disp(int handle, unsigned long long pts, int png_flag);
extern int subtitle_queue_get_disp(int *p_handle, unsigned long long *p_pts, int *p_png_flag);
extern int subtitle_queue_remove_disp(void);

#ifdef __cplusplus
}
#endif

#endif	/* __SUBTITLE_QUEUE_H__ */

