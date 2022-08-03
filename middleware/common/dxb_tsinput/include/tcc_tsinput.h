/*
 * tcc_input.h
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

#ifndef _TCC_TSINPUT_H_
#define _TCC_TSINPUT_H_

#include <tcc_socket_util.h>

#define SOURCE_SOCK0    0x00
#define SOURCE_SOCK1    0x01
#define SOURCE_FILE     0x02

typedef enum SOCKET_START_FLAGS{
	SOCKET_START_FLAG_NONE = 0x0,
	SOCKET_START_FLAG_USR_SOCK = 0x1
} SOCKET_START_FLAGS;

typedef struct {
	int                   hInputDev;
	int                   iSource;
	char                  szPath[1024];
	int                   iStatus;
	pthread_t             hThread;
	int                   iRequestStop;
	int                   iFlag;
	TCCSOCKET_UTIL_HANDLE usrSockHandle;
} TCC_INPUT, *TCC_INPUT_HANDLE;

TCC_INPUT_HANDLE TCCInput_Init(int iFlags);
void TCCInput_Deinit(TCC_INPUT_HANDLE h);
int  TCCInput_Start(TCC_INPUT_HANDLE h);
int  TCCInput_Stop(TCC_INPUT_HANDLE h);
int TCCInput_Write(TCCSOCKET_UTIL_HANDLE *h, unsigned char *data_ptr, unsigned int data_size);

#endif//_TCC_TSINPUT_H_
