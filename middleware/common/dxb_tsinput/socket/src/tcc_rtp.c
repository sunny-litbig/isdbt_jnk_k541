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

#define LOG_TAG	"TCC_RTP"
#include <utils/Log.h>

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

#define		PRINTF	ALOGE

typedef enum
{
	PT_G711_MULAW = 0,
	PT_G711_ALAW = 8,
	PT_G729AB = 18,
	PT_G723_1 = 4,
	PT_G722 = 9,
	PT_TELEPHONE = 101,
	PT_MAXNUM
} RTP_PayloadType;

typedef enum
{
	DTMFSEND_INBAND_RFC2833 = 0,
	DTMFSEND_INBAND_BYPASS,
	DTMFSEND_OUTOFBAND,
	DTMFSEND_MAXMODE,
} DTMFSend_Mode;

#define SWAP32(x)	((((unsigned int)(x) & 0x000000ff) << 24)	|	\
					  	 (((unsigned int)(x) & 0x0000ff00) << 8)	|	\
						 (((unsigned int)(x) & 0x00ff0000) >> 8)	|	\
						 (((unsigned int)(x) & 0xff000000) >> 24))

#define SWAP16(x)	((((unsigned short)(x) & 0x00ff) << 8) |	\
						 (((unsigned short)(x) & 0xff00) >> 8))

int TCRTP_Parse (TCC_RTP_USR_t *psRTPStatus, unsigned char *pucPacket, unsigned int uiPacketSize, unsigned char **pucPayload, unsigned int *puiPayloadSize)
{
	unsigned int rdpos = 0;
	if (pucPacket[0] == 0x47)// && !(uiPacketSize % 188))
	{		
		*pucPayload = pucPacket;
		*puiPayloadSize = uiPacketSize;
		return 1;
	}
	psRTPStatus->Len = uiPacketSize;
	psRTPStatus->Version = (*(pucPacket + rdpos) & 0xc0) >> 6;
	psRTPStatus->Padding = (*(pucPacket + rdpos) & 0x20) >> 5;
	psRTPStatus->Extension = (*(pucPacket + rdpos) & 0x10) >> 4;
	psRTPStatus->CSRC_Cnt = (*(pucPacket + rdpos) & 0x0f);
	rdpos += 1;

	psRTPStatus->Marker = (*(pucPacket + rdpos) & 0x80) >> 7;
	psRTPStatus->PayloadType = (*(pucPacket + rdpos) & 0x7f);
	rdpos += 1;

	memcpy (&psRTPStatus->SequenceNo, (pucPacket + rdpos), 2);
	psRTPStatus->SequenceNo = SWAP16 (psRTPStatus->SequenceNo);
	rdpos += 2;

#if 1
	if(psRTPStatus->SequenceNo && psRTPStatus->prev_SequenceNo && psRTPStatus->SequenceNo != (psRTPStatus->prev_SequenceNo+1))
	{
		ALOGE("%s %d prev_SequenceNo = %d SequenceNo = %d \n", __func__, __LINE__, psRTPStatus->prev_SequenceNo, psRTPStatus->SequenceNo);
	}
	psRTPStatus->prev_SequenceNo = psRTPStatus->SequenceNo;
#endif

	memcpy (&psRTPStatus->TimeStamp, (pucPacket + rdpos), 4);
	psRTPStatus->TimeStamp = SWAP32 (psRTPStatus->TimeStamp);
	rdpos += 4;

	memcpy (&psRTPStatus->SSRC, (pucPacket + rdpos), 4);
	psRTPStatus->SSRC = SWAP32 (psRTPStatus->SSRC);
	rdpos += 4;

	if (psRTPStatus->CSRC_Cnt)
	{
		int       i;

		for (i = 0; i < psRTPStatus->CSRC_Cnt; i++)
		{
			if (i < 15) {
				memcpy (&psRTPStatus->CSRC[i], (pucPacket + rdpos), 4);
				psRTPStatus->TimeStamp = SWAP32 (psRTPStatus->TimeStamp);
			}
			rdpos += 4;
		}
	}

	if (psRTPStatus->Extension)
	{
		unsigned short type = 0;
		unsigned short length = 0;

		memcpy (&type, (pucPacket + rdpos), 2);
		type = SWAP16 (type);
		rdpos += 2;

		memcpy (&length, (pucPacket + rdpos), 2);
		length = SWAP16 (length);
		rdpos += 2;

		if ((rdpos+length) >= uiPacketSize) return -1;
		if (length > 512) memcpy (psRTPStatus->ExtensionHeader, (pucPacket + rdpos), 512);
		else memcpy (psRTPStatus->ExtensionHeader, (pucPacket + rdpos), length);
		rdpos += length;
	}
	
	psRTPStatus->Payload = pucPacket + rdpos;
	psRTPStatus->PayloadLen = psRTPStatus->Len - rdpos;

	*pucPayload = psRTPStatus->Payload;
	*puiPayloadSize = psRTPStatus->PayloadLen;
	return 0;
}
