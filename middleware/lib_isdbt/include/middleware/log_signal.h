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

#ifndef __LOG_SIGNAL_H__
#define __LOG_SIGNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

extern int log_signal_open(char *sPath);
extern int log_signal_close(void);
extern void log_signal_init(void);
extern void log_signal_deinit(void);
extern int log_signal_write(int *p_data);
extern int log_signal_get_status(void);
extern int log_signal_debug_toggle(void);

#ifdef __cplusplus
}
#endif

#endif  //__LOG_SIGNAL_H__
