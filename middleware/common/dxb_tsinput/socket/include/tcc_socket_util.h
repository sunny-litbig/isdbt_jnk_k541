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


#ifndef __TCSOCKET_UTIL_H__
#define __TCSOCKET_UTIL_H__

#include <tcc_rtp.h>
#include <sys/types.h>

typedef void *TCSOCK_HANDLE;

typedef enum SOCKET_ERR_CODE
{
	SOCKET_ERR_OK= 0,
	SOCKET_ERR_ERROR,
	SOCKET_ERR_SOCKET_OPENFAIL,
	SOCKET_ERR_CALLBACK_REGFAIL,
	SOCKET_ERR_READFAIL,
	SOCKET_ERR_MAX
}SOCKET_ERR_CODE;


typedef struct
{
	char aucIPStr[32];
	unsigned int uiPort;	
	unsigned int uiProtocol; //Not yet support.
}TCCSOCKET_SET_IP;

typedef int (*SOCKETCALLBACK) (void *pcInData, unsigned int iInDataSize, void **ppcOutData, unsigned int *piOutDataSize);

typedef struct
{
	TCCSOCKET_SET_IP SocketInfo;
	TCSOCK_HANDLE    SockHandle;
	int              Socket_Thread_Runing;
	pthread_t        Socket_Thread_Id;
	unsigned int     Socket_RecvBufSize;
	unsigned char    *Socket_DataBuf;
	int              Socket_InputDev;
	SOCKETCALLBACK   callback;
	TCC_RTP_USR_t        gRTPStatus;
}TCCSOCKET_UTIL_HANDLE;

extern int TCSOCKUTIL_Read(TCSOCK_HANDLE handle, unsigned char *pucBuffer, int iBufferSize, int iTimeOutMS);
extern void TCSOCKUTIL_Close(TCSOCK_HANDLE handle);
extern int TCSOCKUTIL_Init(TCCSOCKET_UTIL_HANDLE *h, int inputDev);
extern int TCSOCKUTIL_IPSetting(TCCSOCKET_UTIL_HANDLE *h, char *pcIPstr, int iPort, int protocol);
extern TCSOCK_HANDLE TCSOCKUTIL_Open (TCCSOCKET_UTIL_HANDLE *h);
extern int TCSOCKUTIL_Stop(TCCSOCKET_UTIL_HANDLE *h);
extern int TCSOCKUTIL_Start(TCCSOCKET_UTIL_HANDLE *h);
extern int TCSOCKUTIL_Command(int cmd);

#endif // __TCSOCKET_UTIL_H__

