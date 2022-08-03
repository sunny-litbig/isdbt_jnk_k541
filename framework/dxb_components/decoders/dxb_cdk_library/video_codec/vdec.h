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

#ifndef _VDEC_H_
#define _VDEC_H_

#include "./vpu/TCC_VPU_CODEC_EX.h" // VPU video codec
#include "cdk_core.h"
#include "cdk.h"
#include "cdmx.h"

#if defined(TCC_VPU_C5_INCLUDE) || defined(TCC_VPU_C7_INCLUDE)
#define SUPPORT_VCACHE_CTRL
#endif
#if defined(TCC_VPU_C7_INCLUDE)
#define SUPPORT_MJPEG_IN_VPU
#define NEED_SPECIFIC_PROCESS_FOR_MJPEG //use this feature until AVAlgorithm modify.
#endif

#define K_VA 2

#define VPU_ENV_INIT_ERROR		10000
#define VPU_NOT_ENOUGH_MEM		20000

#if defined(_TCC9200_) || defined(_TCC9200S_)
#define AVAILABLE_MAX_WIDTH    1280
#define AVAILABLE_MAX_HEIGHT   720
#else
#define AVAILABLE_MAX_WIDTH    1920
#define AVAILABLE_MAX_HEIGHT   1088
#endif
#define AVAILABLE_MAX_REGION   (AVAILABLE_MAX_WIDTH * AVAILABLE_MAX_HEIGHT)

#define AVAILABLE_MIN_WIDTH    64
#define AVAILABLE_MIN_HEIGHT   64

#define VDEC_INIT										 0	//!< init
#define VDEC_DEC_SEQ_HEADER								 1	//!< seq
#define VDEC_DECODE										 2	//!< decode
#define VDEC_BUF_FLAG_CLEAR								 3	//!< display buffer flag clear
#define VDEC_CLOSE										 4	//!< close
#define VDEC_FINISH										 5	//!< the end of decoding
#define VDEC_DEC_FLUSH_OUTPUT							 6	//!< flush delayed output frame

#define VDEC_GET_RING_BUFFER_STATUS						 7
#define VDEC_FILL_RING_BUFFER							 8 //!< Fill the ring buffer

#define VDEC_UPDATE_WRITE_BUFFER_PTR					 9
#define VDEC_GET_INITIAL_INFO_FOR_STREAMING_MODE_ONLY	10
#define VDEC_GET_INTERMEDIATE_BUF_INFO					11

#define VDEC_SW_RESET                  					12

#define VDEC_HW_RESET									99

#define VDEC_SKIP_FRAME_DISABLE		0	//!< Skip disable
#define VDEC_SKIP_FRAME_EXCEPT_I	1	//!< Skip except I(IDR) picture
#define VDEC_SKIP_FRAME_ONLY_B		2	//!< Skip B picture(skip if nal_ref_idc==0 in H.264)
#define VDEC_SKIP_FRAME_UNCOND		3	//!< Unconditionally Skip one picture

#ifdef INCLUDE_WMV78_DEC
	#define STD_WMV78	8
#endif
#ifdef INCLUDE_SORENSON263_DEC
	#define	STD_SORENSON263	9
#endif

#define VCODEC_MAX		11

//#define VPU_BUFF_COUNT  6
#define VPU_BUFF_COUNT  5

#define AVC_IDR_PICTURE_SEARCH_MODE      0x001
#define AVC_NONIDR_PICTURE_SEARCH_MODE   0x201

//for resolution changing.
//#define SET_FRAMEBUFFER_INTO_MAX

#if defined(HAVE_LINUX_PLATFORM)
typedef struct vdec_user_info_t
{
	unsigned int  bitrate_mbps;			//!< video BitRate (Mbps)
	unsigned int  frame_rate;			//!< video FrameRate (fps)
	unsigned int  m_bJpegOnly;			//!< If this is set, content is jpeg only file (ex. **.jpg)
	unsigned int  jpg_length;
	unsigned int  jpg_ScaleRatio; 			////!< 0 ( Original Size )
							  //!< 1 ( 1/2 Scaling Down )
							  //!< 2 ( 1/4 Scaling Down )
							  //!< 3 ( 1/8 Scaling Down )
	unsigned int bMaxfbMode;
	unsigned int m_iFrameWidth;
	unsigned int m_iFrameHeight;
}vdec_user_info_t;
#endif

typedef struct vdec_init_t
{
	int m_iBitstreamFormat;					//!< bitstream format
	int m_bHasSeqHeader;					//!< sequence header flag
	unsigned int m_RegBaseAddr;				//!< virtual address BIT_BASE
	unsigned int m_BitstreamBufAddr[2];		//!< bitstream buf address : multiple of 4
	unsigned int m_BitWorkAddr[2];			//!< physical[0] and virtual[1] address of a working space of the decoder.
											//!< This working buffer space consists of work buffer, code buffer, and parameter buffer.
	int m_iBitstreamBufSize;				//!< bitstream buf size	   : multiple of 1024
	int	m_iPicWidth;						//!< frame width from demuxer or etc
	int	m_iPicHeight;						//!< frame height from demuxer or etc
	unsigned int m_bEnableUserData;			//!< If this is set, VPU returns userdata.
	unsigned int m_bEnableVideoCache;		//!< video cache
	unsigned int m_bCbCrInterleaveMode;		//!< 0 (chroma separate mode   : CbCr data is written in separate frame memories) \n
											//!< 1 (chroma interleave mode : CbCr data is interleaved in chroma memory)
	unsigned int m_bM4vDeblk;
	unsigned int m_uiDecOptFlags;
	unsigned int m_bFilePlayEnable;			//!< 1:file play mode(default), 0:streaming mode
	unsigned int m_uiMaxResolution;			//!< 0:full-HD, 1:HD, 2:SD

	void* (*m_pfMemcpy )	( void*, const void*, unsigned int );	//!< memcpy
	void  (*m_pfMemset )	( void*, int, unsigned int );			//!< memset
	int   (*m_pfInterrupt ) ( void );								//!< hw interrupt

	unsigned char *m_pExtraData;	//!< codec specific data of asf	( WMV78 Video Decoder only )
	int m_iFourCC;					//!< FourCC info.				( WMV78 Video Decoder only )
	int m_iExtraDataLen;			//!< codec specific data size	( WMV78 Video Decoder only )
	unsigned int m_Reserved;
} vdec_init_t;

typedef struct vdec_info_t
{
	int m_iPicWidth;				//!< {(PicX+15)/16} * 16  (this width  will be used while allocating decoder frame buffers. picWidth  is a multiple of 16)
	int m_iPicHeight;				//!< {(PicY+15)/16} * 16  (this height will be used while allocating decoder frame buffers. picHeight is a multiple of 16)
	unsigned int m_uiFrameRateRes;	//!< decoded picture frame rate residual(number of time units of a clock operating at the frequency[m_iFrameRateDiv] Hz, frameRateInfo(frame per second) = m_uiFrameRateRes/m_uiFrameRateDiv
	unsigned int m_uiFrameRateDiv;	//!< decoded picture frame rate unit number in Hz
	int m_iMinFrameBufferCount;		//!< the minimum number of frame buffers that are required for decoding. application must allocate at least this number of frame buffers.
	int m_iMinFrameBufferSize;		//!< minimum frame buffer size
	int	m_iNormalSliceSize;			//!< recommended size of to save slice. this value is determined as a quater of the memory size for one raw YUV image in KB unit.
	int	m_iWorstSliceSize;			//!< recommended size of to save slice in worst case. this value is determined as a half of the memory size for one raw YUV image in KB unit.

	// H264/AVC only param
	pic_crop_t m_iAvcPicCrop;		//!< represents rectangular window information in a frame
	int m_iAvcConstraintSetFlag[4];	//!< syntax element which is used to make level in H.264. used only in H.264 case.
	int m_iAvcParamerSetFlag;		//!< These are H.264 SPS or PPS syntax element
									//!< [bit 0  ] direct_8x8_inference_flag in H.264 SPS
	int m_iFrameBufDelay;		//!< maximum display frame buffer delay to process reordering in case of H.264

	// MPEG-4 only param
	int m_iM4vDataPartitionEnable;	//!< ( 0: disable   1: enable )
	int m_iM4vReversibleVlcEnable;	//!< ( 0: disable   1: enable )
	int m_iM4vShortVideoHeader;		//!< ( 0: disable   1: enable )
	int m_iM4vH263AnnexJEnable;		//!< ( 0: disable   1: enable )

	// VC-1 only param
	int m_iVc1Psf;					//!< this is only available in VC1 and indicates the value of "Progressive Segmented Frame"

	//! Additional Info
	int m_iProfile;					//!< profile of the decoded stream
	int m_iLevel;					//!< level of the decoded stream
	int m_iInterlace;				//!< when this value is 1, decoded stream will be decoded into both progressive or interlace frame.
									//!< otherwise, all the frames will be progressive.
	int m_iAspectRateInfo;			//!< aspect rate information. if this value is 0, then aspect ratio information is not present
	int m_iReportErrorReason;		//!< reports reason of 'RETCODE_CODEC_SPECOUT' or 'RETCODE_INVALID_STRIDE' error

} vdec_info_t;

typedef struct vdec_input_t
{
	unsigned char* m_pInp[2];			//!< input data
	int m_iInpLen;						//!< input data len
	unsigned char* m_UserDataAddr[2];	//!< Picture Layer User-data address
	int m_iUserDataBufferSize;			//!< Picture Layer User-data Size

	int m_iFrameSearchEnable;			//!< I-frame Search Mode. \n
										//!< If this option is enabled, the decoder skips the frame decoding until decoder meet IDR(and/or I)-picture for H.264 or I-frame.\n
										//!< If this option is enabled, m_iSkipFrameMode option is ignored.\n
										//!< 0 ( Disable ) \n
										//!< 1 ( Enable : search IDR-picture for H.264 or I-frame ) \n
										//!< 513 ( Enable : search I(or IDR)-picture for H.264 or I-frame )

	int m_iSkipFrameMode;				//!< Skip Frame Mode \n
										//!< 0 ( Skip disable ) \n
										//!< 1 ( Skip except I(IDR) picture ) \n
										//!< 2 ( Skip B picture : skip if nal_ref_idc==0 in H.264 ) \n
										//!< 3 ( Unconditionally Skip one picture )

	int m_iSkipFrameNum;				//!< Number of skip frames (for I-frame Search Mode or Skip Frame Mode) \n
										//!< When this number is 0, m_iSkipFrameMode option is disabled.

	int m_iIsThumbnail; 				//!< For checking size of memory while thumbnail extracting on sequence header init
} vdec_input_t;

#define COMP_Y 0
#define COMP_U 1
#define COMP_V 2
typedef struct vdec_output_t
{
	dec_output_info_t m_DecOutInfo;
	unsigned char* m_pDispOut[2][3];	//!< physical[0] and virtual[1] display  address of Y, Cb, Cr component
	unsigned char* m_pCurrOut[2][3];	//!< physical[0] and virtual[1] current  address of Y, Cb, Cr component
	unsigned char* m_pPrevOut[2][3];	//!< physical[0] and virtual[1] previous address of Y, Cb, Cr component
	dec_initial_info_t* m_pInitialInfo;
} vdec_output_t;


typedef struct vdec_ring_buffer_set_t
{
	unsigned char *m_pbyBuffer;
	unsigned long  m_ulBufferSize;
} vdec_ring_buffer_set_t;

typedef struct vdec_ring_buffer_out_t
{
	unsigned long  m_ulAvailableSpaceInRingBuffer;
	unsigned int m_ptrReadAddr_PA;
	unsigned int m_ptrWriteAddr_PA;
} vdec_ring_buffer_out_t;

int
dxb_vdec_vpu( int iOpCode, int* pHandle, void* pParam1, void* pParam2, void* pParam3 );

#ifdef INCLUDE_WMV78_DEC
int
vdec_WMV78( int iOpCode, int* pHandle, void* pParam1, void* pParam2 );
#endif

#ifdef INCLUDE_SORENSON263_DEC
int
vdec_sorensonH263dec( int iOpCode, int* pHandle, void* pParam1, void* pParam2 );
#endif

#endif //_VDEC_H_
