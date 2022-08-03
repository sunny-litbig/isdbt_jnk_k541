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


#ifndef __SUBTITLE_QUEUE_POS_H__
#define __SUBTITLE_QUEUE_POS_H__

#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************
DEFINITION OF STRUCTURE
****************************************************************************/
extern int subtitle_queue_pos_init(void);
extern void subtitle_queue_pos_exit(void);
extern int subtitle_queue_pos_getcount(int type);
extern int subtitle_queue_pos_put(int type, int x, int y, int w, int h);
extern int subtitle_queue_pos_get(int type, int *x, int *y, int *w, int *h);
extern int subtitle_queue_pos_peek_nth(int type, int index, int *x, int *y, int *w, int *h);
extern int subtitle_queue_pos_remove_all(int type);

#ifdef __cplusplus
}
#endif

#endif	/* __SUBTITLE_QUEUE_POS_H__ */
