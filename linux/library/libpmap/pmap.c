/*
 * Copyright (C) 2010 Telechips, Inc.
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

#define LOG_TAG "pmap"

#include <utils/Log.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


#include "pmap.h"

#if 1
#include <limits.h>
#include <tccmisc_drv.h>

#define TCCMISC_DEV_NAME    "/dev/tccmisc"

int pmap_get_info(const char *name, pmap_t *mem)
{
	FILE* tccmisc_fd = NULL;
	struct tccmisc_user_t tccmisc_info;
	
	tccmisc_fd = open(TCCMISC_DEV_NAME, O_RDWR | O_NDELAY);
	
	if (tccmisc_fd < 0)
	{
		ALOGE("%s driver open error!! \n", TCCMISC_DEV_NAME);
		return -1;
	}
	
	memset(&tccmisc_info, 0, sizeof(struct tccmisc_user_t));
	memcpy(&tccmisc_info.name, name, sizeof(tccmisc_info.name));
	
	if (ioctl(tccmisc_fd, IOCTL_TCCMISC_PMAP, &tccmisc_info) < 0) {
		ALOGE("tccmisc ioctl error! \n");
		close(tccmisc_fd);
		return -1;
	}

	ALOGV("name = %s, BaseAddress = 0x%llX Size = 0x%llX\n", tccmisc_info.name, 
	tccmisc_info.base, tccmisc_info.size);

	if (tccmisc_info.base <= UINT_MAX)
	{
		mem->base = (unsigned int)tccmisc_info.base;
	}
	else
	{
		ALOGE("tccmisc_info.base is over unsiengd int range. \n");
	}

	if (tccmisc_info.size <= UINT_MAX)
	{
		mem->size = (unsigned int)tccmisc_info.size;
	}
	else
	{
		ALOGE("tccmisc_info.size is over unsiengd int range. \n");
	}

	close(tccmisc_fd);

    return 0;
}
#else
#define PATH_PROC_PMAP	"/proc/pmap"

int pmap_get_info(const char *name, pmap_t *mem)
{
    int fd;
    int matches;
    char buf[2048];
    const char *p;
    ssize_t nbytes;
    unsigned int base_addr;
    unsigned int size;
    char s[128];

    fd = open(PATH_PROC_PMAP, O_RDONLY);
    if (fd < 0)
	return 0;

    nbytes = read(fd, buf, sizeof(buf));
    close(fd);

    p = buf;
    while (1) {
	matches = sscanf(p, "0x%x 0x%x %s", &base_addr, &size, s);
	if (matches == 3 && !strcmp(name, s)) {
	    ALOGV("requested physical memory '%s' (base=0x%x size=0x%x)",
		 name, base_addr, size);
	    mem->base = base_addr;
	    mem->size = size;
	    return 1;
	}
	p = strchr(p, '\n');
	if (p == NULL)
	    break;
	p++;
    }
    //ALOGE("can't get physical memory '%s'", name);
    return 0;
}
#endif
