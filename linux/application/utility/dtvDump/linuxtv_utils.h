/*
 * Copyright (C) 2012 Telechips, Inc.  All rights reserved.
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

#ifndef __LINUXTV_UTILS_H__     
#define __LINUXTV_UTILS_H__

typedef void LINUXTV;

LINUXTV* linuxtv_open(int devid, int *pid, int ipidnum);
void linuxtv_close(LINUXTV *linuxtv);
int linuxtv_start(LINUXTV *linuxtv);
void linuxtv_stop(LINUXTV *linuxtv);
int linuxtv_read(LINUXTV *linuxtv, unsigned char *buff, int readsize);

#endif//__LINUXTV_UTILS_H__
