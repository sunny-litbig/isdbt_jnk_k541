/****************************************************************************

Copyright (C) 2013 Telechips Inc.


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions andlimitations under the License.

****************************************************************************/

/****************************************************************************

Revision History

****************************************************************************

****************************************************************************/
#ifndef _TCC_DXB_TS_PARSER_H_
#define _TCC_DXB_TS_PARSER_H_

#ifndef NULL
#define NULL 0
#endif

#define PACKETSIZE				(188)

#define TRANS_ERROR	 0x80
#define PAY_START	 0x40
#define PAYLOAD		 0x10
#define ADAPT_FIELD	 0x20

typedef enum
{
	STREAM_ID_PROGRAM_STREAM_MAP = 0xBC,
	STREAM_ID_PADDING_STREAM = 0xBE,
	STREAM_ID_PRIVATE_STREAM_2 = 0xBF,
	STREAM_ID_ECM_STREAM = 0xF0,
	STREAM_ID_EMM_STREAM = 0xF1,
	STREAM_ID_DSMCC = 0xF2,
	STREAM_ID_ITU_T_REC_H222_1_TYPE_E = 0xF8,
	STREAM_ID_PROGRAM_STREAM_DIRECTORY = 0xFF,
	STREAM_ID_MAX
} MPEGPES_STREAM_ID;

typedef struct
{
	unsigned char     ext_flag:1;
	unsigned char     private_data_flag:1;
	unsigned char     splicing_point_flag:1;
	unsigned char     OPCR:1;
	unsigned char     PCR:1;
	unsigned char     priority_indicator:1;
	unsigned char     random_access_indicator:1;
	unsigned char     discontinuity_indicator:1;
} MpegTsAdaptionFlg;

typedef struct
{
	int			length;
	long long	PCR;
	MpegTsAdaptionFlg flag;
} MpegTsAdaptation;

typedef struct
{
	unsigned short    PES_extension_flag:1;
	unsigned short    PES_CRC_flag:1;
	unsigned short    additional_copy_info_flag:1;
	unsigned short    DSM_trick_mode_flag:1;
	unsigned short    ES_rate_flag:1;
	unsigned short    ESCR_flag:1;
	unsigned short    PTS_DTS_flags:2;
	unsigned short    original_or_copy:1;
	unsigned short    copyright:1;
	unsigned short    data_alignment_indicator:1;
	unsigned short    PES_priority:1;
	unsigned short    PES_scrambling_control:2;
	unsigned short    dummy_pes_data:2;
} MpegPesFlag;

typedef struct
{
	int       		stream_id;
	int       		length;
	int       		header_length;
	int       		payload_size;
	char     		*payload;
	MpegPesFlag 	flag;
	long long     	pts;
	long long     	dts;
} MpegPesHeader;

typedef struct
{
	int nPID;
	int nPayLoadStart;
	int nNextCC;
	int nPTS_flag;
	int nTSScrambled;
	int nPESScrambled;
	int nPrevScrambled;
	int nScrambleDetectPattern;
	MpegTsAdaptation nAdaptation;
	int nContentSize;
	unsigned long long nPTS;
	unsigned long long nPrevPTS;
	unsigned char *pBuf;
	int nLen;
	//for debug
	int iPacketCount;
	int iCountTSError; 
	int iCountDiscontinuityError;
} TS_PARSER;

int parseTs(TS_PARSER *pTsParser, unsigned char *buf, int len, int bOnlyAdapt);

#endif
