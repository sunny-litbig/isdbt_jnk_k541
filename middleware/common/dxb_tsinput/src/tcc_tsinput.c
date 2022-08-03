/*
 * tcc_input.c
 *
 *  Copyright (C) 2015, Telechips, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

//#define LOG_NDEBUG 0
#define LOG_TAG	"TCC_TSINPUT"
#include <utils/Log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#if defined(HAVE_LINUX_PLATFORM)
#ifndef AF_INET
#define AF_INET 2
#endif
#endif

#include "tcc_tsinput.h"
#include <tcc_dxb_internal.h>

#define INTERNAL0_DEV "/dev/tcc_dxb_internal0"
#define INTERNAL1_DEV "/dev/tcc_dxb_internal1"

/**************************************************************************
						DEFINE
**************************************************************************/
enum {
	STOPPED = 0,
	STARTED,
};


/**************************************************************************
						VARIABLE DEFINE
**************************************************************************/


/**************************************************************************
						STATIC FUNCTION DEFINE
**************************************************************************/
static void* FileInput_Thread(void *pvArg)
{
	TCC_INPUT_HANDLE h = (TCC_INPUT_HANDLE) pvArg;
	FILE *hFile = fopen(h->szPath, "rb");

	if (hFile == NULL)
		return NULL;

	while (h->iRequestStop == 0)
	{
		usleep(1000);
	}

	fclose(hFile);

	return NULL;
}

static int FileInput_Start(TCC_INPUT_HANDLE h)
{
	h->iRequestStop = 0;
	return pthread_create(&h->hThread, NULL, FileInput_Thread, (void*)h);
}

static int FileInput_Stop(TCC_INPUT_HANDLE h)
{
	h->iRequestStop = 1;
	pthread_join(h->hThread, NULL);
	return 0;
}

static int SockInput_Start(TCC_INPUT_HANDLE h)
{
	struct sockaddr_in addr;
	int i = strlen(h->szPath);

	while (--i >= 0)
	{
		if (h->szPath[i] == ':')
		{
			addr.sin_family = AF_INET;
			addr.sin_port = htons(atoi(&h->szPath[i+1]));
			h->szPath[i] = 0;
			if (inet_aton(h->szPath, &addr.sin_addr))
			{
				return ioctl(h->hInputDev, TCC_DXB_INTERNAL_START, &addr);
			}
		}
	}
	return -1;
}

static int SockInput_Stop(TCC_INPUT_HANDLE h)
{
	return ioctl(h->hInputDev, TCC_DXB_INTERNAL_END, 0);
}

static int SockInput_Start_usr(TCC_INPUT_HANDLE h)
{
	int i = strlen(h->szPath);

	while (--i >= 0)
	{
		if (h->szPath[i] == ':')
		{
			h->szPath[i] = 0;
			TCSOCKUTIL_Init(&h->usrSockHandle, h->hInputDev);
			TCSOCKUTIL_IPSetting(&h->usrSockHandle, h->szPath, atoi(&h->szPath[i+1]), 2);
			return TCSOCKUTIL_Start(&h->usrSockHandle);
		}
	}
	return -1;
}

static int SockInput_Stop_usr(TCC_INPUT_HANDLE h)
{
	return TCSOCKUTIL_Stop(&h->usrSockHandle);
}

/**************************************************************************
						FUNCTION DEFINE
**************************************************************************/
TCC_INPUT_HANDLE TCCInput_Init(int iSource)
{
	TCC_INPUT_HANDLE h = (TCC_INPUT_HANDLE)tcc_mw_malloc(__FUNCTION__, __LINE__, sizeof(TCC_INPUT));
	memset(h, 0, sizeof(TCC_INPUT));

	ALOGD("In %s", __func__);

	if (h)
	{
		system("echo 1310720 > /proc/sys/net/core/rmem_max");
		if (iSource == SOURCE_SOCK1)
		{
			h->hInputDev = open(INTERNAL1_DEV, O_RDWR);
		}
		else
		{
			h->hInputDev = open(INTERNAL0_DEV, O_RDWR);
		}
		if (h->hInputDev >= 0)
		{
			h->hThread = 0;
			h->iSource = iSource;
			h->iStatus = STOPPED;
		}
		else
		{
			tcc_mw_free(__FUNCTION__, __LINE__, h);
			h = NULL;
		}
	}

	ALOGD("Out %s - h = 0x%x", __func__, (unsigned int)h);

	return h;
}

void TCCInput_Deinit(TCC_INPUT_HANDLE h)
{
	ALOGD("In %s", __func__);

	if (h)
	{
		if (h->iStatus == STARTED)
		{
			TCCInput_Stop(h);
		}
		close(h->hInputDev);
		tcc_mw_free(__FUNCTION__, __LINE__, h);
	}

	ALOGD("Out %s", __func__);
}

int TCCInput_Start(TCC_INPUT_HANDLE h)
{
	int iRet = -1;

	ALOGD("In %s - %s", __func__, h->szPath);

	if (h && h->iStatus == STOPPED)
	{
		if (h->iSource == SOURCE_SOCK0 || h->iSource == SOURCE_SOCK1)
		{
			if(h->iFlag&SOCKET_START_FLAG_USR_SOCK) {
				iRet = SockInput_Start_usr(h);
			}
			else {
				iRet = SockInput_Start(h);
			}
		}
		else if (h->iSource == SOURCE_FILE)
		{
			iRet = FileInput_Start(h);
		}

		if (iRet == 0)
		{
			h->iStatus = STARTED;
		}
	}

	ALOGD("Out %s - iRet = %d", __func__, iRet);

	return iRet;
}

int TCCInput_Stop(TCC_INPUT_HANDLE h)
{
	int iRet = -1;

	ALOGD("In %s", __func__);

	if (h && h->iStatus == STARTED)
	{
		if (h->iSource == SOURCE_SOCK0 || h->iSource == SOURCE_SOCK1)
		{
			if(h->iFlag&SOCKET_START_FLAG_USR_SOCK) {
				iRet = SockInput_Stop_usr(h);
			}
			else {
				iRet = SockInput_Stop(h);
			}
		}
		else if (h->iSource == SOURCE_FILE)
		{
			iRet = FileInput_Stop(h);
		}
		if (iRet == 0)
		{
			h->iStatus = STOPPED;
		}
	}

	ALOGD("Out %s - iRet = %d", __func__, iRet);

	return iRet;
}


int TCCInput_Write(TCCSOCKET_UTIL_HANDLE *h, unsigned char *data_ptr, unsigned int data_size)
{
	int err = 0;
	err = write(h->Socket_InputDev, data_ptr, data_size);
	if(err < 0) {
		ALOGE("Wrtie Packet ret[%d] \n", err);
	}
	return err;
}

