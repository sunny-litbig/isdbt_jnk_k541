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

#ifndef __SUBTITLE_SCALER_H__
#define __SUBTITLE_SCALER_H__

#ifdef __cplusplus
extern "C" {
#endif

extern int subtitle_sw_scaler(unsigned long* srcBuf, int src_w, int src_h, unsigned long* dstBuf, int dst_w, int dst_h);
extern int subtitle_hw_scaler(unsigned int* srcBuf, int src_x, int src_y, int src_w, int src_h, int src_pw, int src_ph, unsigned int* dstBuf, int dst_x, int dst_y, int dst_w, int dst_h, int dst_pw, int dst_ph);

#ifdef __cplusplus
}
#endif

#endif	/* __SUBTITLE_SCALER_H__ */

