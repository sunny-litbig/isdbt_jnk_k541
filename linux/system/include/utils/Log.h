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
 
#ifndef __LOG_H__
#define __LOG_H__

#ifndef LOG_NDEBUG
#ifdef NDEBUG
#define LOG_NDEBUG 1
#else
#define LOG_NDEBUG 0
#endif
#endif

#ifndef printf
#include <stdio.h>  	/* for printf() */
#endif

#include "LB_debug_log.h"


#ifndef ALOGV
#define ALOGV(msg...) LB_Debug_Message(LB_LOG_LEVEL_VERBOSE, msg)
#endif

#ifndef ALOGD
#define ALOGD(msg...) LB_Debug_Message(LB_LOG_LEVEL_DEBUG, msg)
#endif

#ifndef ALOGI
#define ALOGI(msg...) LB_Debug_Message(LB_LOG_LEVEL_INFO, msg)
#endif

#ifndef ALOGW
#define ALOGW(msg...) LB_Debug_Message(LB_LOG_LEVEL_WARN, msg)
#endif

#ifndef ALOGE
#define ALOGE(msg...) LB_Debug_Message(LB_LOG_LEVEL_ERROR, msg)
#endif


#ifndef LLOGV
#define LLOGV(msg...) LB_Debug_Message(LB_LOG_LEVEL_VERBOSE, msg)
#endif

#ifndef LLOGD
#define LLOGD(msg...) LB_Debug_Message(LB_LOG_LEVEL_DEBUG, msg)
#endif

#ifndef LLOGI
#define LLOGI(msg...) LB_Debug_Message(LB_LOG_LEVEL_INFO, msg)
#endif

#ifndef LLOGW
#define LLOGW(msg...) LB_Debug_Message(LB_LOG_LEVEL_WARN, msg)
#endif

#ifndef LLOGE
#define LLOGE(msg...) LB_Debug_Message(LB_LOG_LEVEL_ERROR, msg)
#endif

#ifndef LOG_TAG
#define LOG_TAG NULL
#endif

#endif // __LOG_H__