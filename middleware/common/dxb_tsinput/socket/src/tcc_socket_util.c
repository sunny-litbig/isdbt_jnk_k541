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

#define LOG_TAG	"TCC_SOCKET_UTIL"
#include <utils/Log.h>
#include <pthread.h>

#define LOG_DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <netdb.h>

#include "tcc_socket.h"
#include "tcc_socket_util.h"
#include "tcc_rtp.h"

#if	LOG_DEBUG
#define DEBUG_PRINTF		ALOGE
#else
#define DEBUG_PRINTF		
#endif

#define	TS_PACKETSIZE 		(200)//	(188)
#define SOCKET_RECVBUFSIZE	512*1024
#define SocketDataReadSize	64*1024

static unsigned long get_ipv4_addr (const char *ip)
{
	DEBUG_PRINTF("%s %d \n", __func__, __LINE__);

	unsigned long sock_addr = 0;
	struct addrinfo hints, *res0;
	struct sockaddr *res;

	if (!ip)
	{
		return (0);
	}

	memset (&hints, 0, sizeof (hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	res0 = (struct addrinfo *) 0;
	if (!getaddrinfo (ip, NULL, &hints, &res0))
	{
		if (res0)
		{
			res = (struct sockaddr *) res0->ai_addr;
			sock_addr = (((struct sockaddr_in *) res)->sin_addr).s_addr;
		}
	}
	if (res0)
	{
		freeaddrinfo (res0);
	}

	return ntohl (sock_addr);
}

static int check_ipv4_type (const char *p_ip)
{
	DEBUG_PRINTF("%s %d \n", __func__, __LINE__);

	int       i_result = (-1);
	unsigned long int ul_ip = 0;

	if (p_ip)
	{
		ul_ip = get_ipv4_addr (p_ip);

		/* class select */
		if ((0x1000001 <= ul_ip) && (ul_ip <= 0x7efffffe))
		{
			i_result = 1;
		}
		else if ((0x80010001 <= ul_ip) && (ul_ip <= 0xbffffffe))
		{
			i_result = 2;
		}
		else if ((0xc0000101 <= ul_ip) && (ul_ip <= 0xdffffefe))
		{
			i_result = 3;
		}
		else if ((0xe0000000 <= ul_ip) && (ul_ip <= 0xefffffff))
		{
			i_result = 4;	/* multicasting */
		}
		else if ((0xf0000000 <= ul_ip) && (ul_ip <= 0xfefffffe))
		{
			i_result = 5;	/* reserved */
		}
	}

	return i_result;
}

/**************************************************************************
FUNCTION NAME : TCSOCKUTIL_SetOpt
DESCRIPTION : data read
INPUT	 :	handle :  socket handle
			iLevel :
			iOptName :
			pOptVal :
OUTPUT	: ts data
return	: read size
**************************************************************************/
static int TCSOCKUTIL_SetOpt(TCSOCK_HANDLE handle, int iLevel, int iOptName, const void *pOptVal)
{
	TCSocket *p_sock =	(TCSocket *)handle;
	socklen_t	len = 0;

	len = sizeof(pOptVal);
	return p_sock->SetSockOpt(p_sock, iLevel, iOptName, pOptVal, len);
}

/**************************************************************************
FUNCTION NAME : TCSOCKUTIL_Read
DESCRIPTION : data read
INPUT	 :	handle :  socket handle
			pucBuffer :  data buf
			iBufferSize :  data read size
			iTimeOutMS : read timeout (msec)
OUTPUT	: ts data
return	: read size
**************************************************************************/
int TCSOCKUTIL_Read(TCSOCK_HANDLE handle, unsigned char *pucBuffer, int iBufferSize, int iTimeOutMS)
{
	TCSocket *p_sock =	(TCSocket *)handle;
	return p_sock->Recv(p_sock, (BYTE *)pucBuffer, iBufferSize, iTimeOutMS);
}

/**************************************************************************
FUNCTION NAME : TCSOCKUTIL_Close
DESCRIPTION : socket close
INPUT	 :	handle :  socket handle
OUTPUT	:
return	: DxB_ERR_CODE
**************************************************************************/
void TCSOCKUTIL_Close(TCSOCK_HANDLE handle)
{
	DEBUG_PRINTF("%s %d \n", __func__, __LINE__);

	TCSocket *p_sock =	(TCSocket *)handle;
	DELETE_TCSOCK(p_sock);
}

/**************************************************************************
FUNCTION NAME : TCSOCKETUTIL_Thread
DESCRIPTION : data read & data send
INPUT	 :  
OUTPUT  : 
return  : 
**************************************************************************/
static void TCSOCKUTIL_Thread(void *args)
{
	int		data_readsize =0;
	int		ret =0;
	unsigned char *pucPayload;
	unsigned int  uiPayloadSize;

	TCCSOCKET_UTIL_HANDLE *h = (TCCSOCKET_UTIL_HANDLE *)args;

	DEBUG_PRINTF("%s %d inputDev[%d]\n", __func__, __LINE__, h->Socket_InputDev);

	while(h->Socket_Thread_Runing)
	{
		if(h->SockHandle)
		{
			data_readsize = TCSOCKUTIL_Read(h->SockHandle, h->Socket_DataBuf, SocketDataReadSize, 500);

			if(data_readsize>0)
			{
				ret = TCRTP_Parse(&h->gRTPStatus, h->Socket_DataBuf, data_readsize, &pucPayload, &uiPayloadSize);
				if(ret < 0)
				{
					ALOGE ("Received Packet Error!!! %d", ret); 
				}	
				else
				{
					if(h->callback) {
						h->callback((void *)pucPayload, uiPayloadSize, (void **)&pucPayload, &uiPayloadSize);
					}
					ret = TCCInput_Write(h, pucPayload, uiPayloadSize);

					if(ret < 0) {
						ALOGE("Wrtie Packet ret[%d] errno[%d]", ret, errno);
					}
				}
			}
		}
		else
			usleep(5000);
		
	}

	DEBUG_PRINTF("%s %d TCSOCKUTIL_Thread join\n", __func__, __LINE__);
}

/**************************************************************************
FUNCTION NAME : TCSOCKUTIL_Init
DESCRIPTION : socket init
INPUT	 :  
OUTPUT  : 
return  : 
**************************************************************************/
int TCSOCKUTIL_Init(TCCSOCKET_UTIL_HANDLE *h, int inputDev)
{
	DEBUG_PRINTF("%s %d \n", __func__, __LINE__);
	int err = SOCKET_ERR_OK;

	h->Socket_Thread_Runing = 0;
	h->Socket_Thread_Id = 0;
	h->SockHandle = NULL;
	h->Socket_DataBuf = NULL;
	h->Socket_RecvBufSize = SOCKET_RECVBUFSIZE;
	h->Socket_InputDev = inputDev;
	
	return SOCKET_ERR_OK;
}

/**************************************************************************
FUNCTION NAME : TCSOCKUTIL_IPSetting
DESCRIPTION : socket ip setting
INPUT	 :  char* ip(ex: 239.1.1.1)
		    int portnum (ex:3990)	
OUTPUT  : 
return  : 
**************************************************************************/
int TCSOCKUTIL_IPSetting(TCCSOCKET_UTIL_HANDLE *h, char *pcIPstr, int iPort, int protocol)
{
	DEBUG_PRINTF("%s %d \n", __func__, __LINE__);
	int err = SOCKET_ERR_OK;

	strcpy(h->SocketInfo.aucIPStr, pcIPstr);
	h->SocketInfo.uiPort = iPort;
	h->SocketInfo.uiProtocol = protocol;

	return err;
}

/**************************************************************************
FUNCTION NAME : TCSOCKUTIL_Open
DESCRIPTION : socket open
INPUT	 :
OUTPUT	:
return	: DxB_ERR_CODE
**************************************************************************/
TCSOCK_HANDLE TCSOCKUTIL_Open (TCCSOCKET_UTIL_HANDLE *h)
{
	DEBUG_PRINTF("%s %d \n", __func__, __LINE__);
	TCSocket *p_sock = NEW_TCSOCK ();
	socklen_t len = 0;
	int ret = 0;

	if(p_sock == NULL)
		return NULL;
	
	if(p_sock->Init (p_sock, _UDP_)) {
		if(p_sock->Bind (p_sock, h->SocketInfo.uiPort, h->SocketInfo.aucIPStr)) {
			if(p_sock->SetRemoteAddr (p_sock, h->SocketInfo.aucIPStr, h->SocketInfo.uiPort)) {
				if(check_ipv4_type (h->SocketInfo.aucIPStr) == 4) {
					if(p_sock->Join_M (p_sock))
					{
						socklen_t	len = 0;
						unsigned int Default_Socket_RecvBufSize;
						len = sizeof(Default_Socket_RecvBufSize);

						ret = TCSOCKUTIL_SetOpt(p_sock, SOL_SOCKET, SO_RCVBUF, &h->Socket_RecvBufSize);
						if(ret != TRUE)
							ALOGE("setsockopt Error ret = %d \n", ret );
						ret = p_sock->GetSockOpt (p_sock, SOL_SOCKET, SO_RCVBUF, &Default_Socket_RecvBufSize, &len);
						if(ret != TRUE)
							ALOGE("getsockopt Error ret = %d \n", ret );
						ALOGE("%s receive socket buffer size[%d]", __func__, Default_Socket_RecvBufSize);
						return p_sock;
					}		
				}
				else
				{
					ret = TCSOCKUTIL_SetOpt(p_sock, SOL_SOCKET, SO_RCVBUF, &h->Socket_RecvBufSize);
					if(ret != TRUE)
						ALOGE("setsockopt Error ret = %d \n", ret );

					return p_sock;
				}
			}	
		}
	}	
	ALOGE("%s errno[%d]", __func__, errno);
	DELETE_TCSOCK(p_sock);
	return SOCKET_ERR_OK;
}

/**************************************************************************
FUNCTION NAME : TCSOCKUTIL_Stop
DESCRIPTION : socket close, destroy thread
INPUT	 :
OUTPUT	:
return	:
**************************************************************************/
int TCSOCKUTIL_Stop(TCCSOCKET_UTIL_HANDLE *h)
{
	DEBUG_PRINTF("%s %d \n", __func__, __LINE__);
	int err = SOCKET_ERR_OK;

	h->Socket_Thread_Runing = 0;

	if(h->SockHandle != NULL)
		TCSOCKUTIL_Close(h->SockHandle);
	h->SockHandle = NULL;

	if(h->Socket_Thread_Id != 0)
		pthread_join(h->Socket_Thread_Id, NULL);
	h->Socket_Thread_Id = 0;
	if(h->Socket_DataBuf != NULL)
		tcc_mw_free(__FUNCTION__, __LINE__, h->Socket_DataBuf);
	h->Socket_DataBuf = NULL;

	return err;
}

/**************************************************************************
FUNCTION NAME : TCSOCKUTIL_Start
DESCRIPTION : socket open, make data_read & data_send thread
INPUT	 :
OUTPUT  :
return  :
**************************************************************************/
int TCSOCKUTIL_Start(TCCSOCKET_UTIL_HANDLE *h)
{
	DEBUG_PRINTF("%s %d \n", __func__, __LINE__);
	int iStatus =0;
	int err = SOCKET_ERR_OK;

	if(h->SockHandle != NULL)
	{
		TCSOCKUTIL_Stop(h);
	}

	h->SockHandle  = TCSOCKUTIL_Open(h);

	DEBUG_PRINTF("%s %d SockHandle = %p \n", __func__, __LINE__, h->SockHandle);
	DEBUG_PRINTF("%s %d ip = %s port = %d \n", __func__, __LINE__, h->SocketInfo.aucIPStr, h->SocketInfo.uiPort);

	if(h->SockHandle == NULL)
	{
		return SOCKET_ERR_SOCKET_OPENFAIL;
	}

	h->Socket_DataBuf = tcc_mw_malloc(__FUNCTION__, __LINE__, SocketDataReadSize + TS_PACKETSIZE);
	if (h->Socket_DataBuf == NULL)
	{
		ALOGE ("CANNOT Alloc Read Buffer : Size(%d)!!!\n", SocketDataReadSize);
		return SOCKET_ERR_ERROR;
	}

	memset(&h->gRTPStatus, 0x0, sizeof(TCC_RTP_USR_t));

	h->Socket_Thread_Runing =1;
	if ((iStatus = pthread_create(&h->Socket_Thread_Id, NULL, (void*)&TCSOCKUTIL_Thread, (void *)h)))
	{
		ALOGE("Error: fail to create Socket_Thread, status=%d\n", iStatus);
		return SOCKET_ERR_ERROR;
	}

	return err;
}

/**************************************************************************
FUNCTION NAME : TCSOCKUTIL_Stop
DESCRIPTION : socket close, destroy thread
INPUT	 :  
OUTPUT  : 
return  : 
**************************************************************************/
int TCSOCKUTIL_Command(int cmd)
{
	DEBUG_PRINTF("%s %d \n", __func__, __LINE__);
	int err = SOCKET_ERR_OK;

	switch(cmd)
	{

		default:
			break;
	}

	return err;
}

