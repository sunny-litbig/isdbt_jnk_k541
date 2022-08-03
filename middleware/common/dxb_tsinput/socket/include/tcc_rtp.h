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


#ifndef __TCSOCKET_RTP_H__
#define __TCSOCKET_RTP_H__

typedef struct
{
	int 	  Len;
	unsigned char Version;
	unsigned char Padding;
	unsigned char Extension;
	unsigned char CSRC_Cnt;
	unsigned char Marker;
	unsigned char PayloadType;
	unsigned short SequenceNo;
	unsigned int TimeStamp;
	unsigned int SSRC;
	unsigned int CSRC[15];
	unsigned char ExtensionHeader[512];
	unsigned int PayloadLen;
	unsigned char *Payload;
	unsigned int prev_SequenceNo;
} TCC_RTP_USR_t;

int TCRTP_Parse (TCC_RTP_USR_t *psRTPStatus, unsigned char *pucPacket, unsigned int uiPacketSize, unsigned char **pucPayload, unsigned int *puiPayloadSize);
#endif // __TCSOCKET_RTP_H__

