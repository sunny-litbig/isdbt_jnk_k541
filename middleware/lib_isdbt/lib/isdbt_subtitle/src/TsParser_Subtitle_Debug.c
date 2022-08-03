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

#if defined(HAVE_LINUX_PLATFORM)
#define LOG_TAG "TsParser_Subtitle"
#include <utils/Log.h>

#define LOGE 	ALOGE
#endif

#include <stdio.h>
#include <stdarg.h>
#include "TsParser_Subtitle_Debug.h"

static unsigned int debug_level = DBG_ERR;

void subtitle_lib_set_debug_level(unsigned int level)
{
	debug_level = (level == 0)?DBG_ERR:level;
}

void LIB_DBG(const int level, const char *format, ...)
{
	va_list arg;
	char buf[256];

	if(debug_level & level){
		va_start(arg, format);
		vsprintf(buf, format, arg);
		va_end(arg);

		LOGE("%s", buf);
	}
}
