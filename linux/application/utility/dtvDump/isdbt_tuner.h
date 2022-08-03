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

#ifndef __ISDBT_TUNER_H__     
#define __ISDBT_TUNER_H__

OMX_COMPONENTTYPE* isdbt_tuner_open(int iFreq, int iBW);
void isdbt_tuner_close(OMX_COMPONENTTYPE *pOpenmaxStandComp);

#endif//__ISDBT_TUNER_H__
