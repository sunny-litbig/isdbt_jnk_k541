/****************************************************************************

Copyright (C) 2013 Telechips Inc.


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions 
andlimitations under the License.

****************************************************************************/

/****************************************************************************

  Revision History

 ****************************************************************************

 ****************************************************************************/
#define LOG_NDEBUG 0
#define LOG_TAG	"DXB_INTERFACE_REMOTEPLAY"
#include <OMX_Core.h>
#include <OMX_Component.h>
#include <OMX_Types.h>

#include <utils/Log.h>
#include <user_debug_levels.h>
#include <omx_base_component.h>

#include "tcc_dxb_interface_type.h"
#include "tcc_dxb_interface_omxil.h"
#include "tcc_dxb_interface_remoteplay.h"

#ifdef      ENABLE_REMOTE_PLAYER

#ifdef      ENABLE_TCC_TRANSCODER
#include "mediastream.h"
static int giStartTransCoder=0;
void TSOuputCallback(unsigned char *pbuffer, long len, long pts, unsigned long estype)
{
//    ALOGD("%s:%d:[%d:%d][0x%X][0x%X][0x%X][0x%X]", __func__, __LINE__, len, estype, pbuffer[0],pbuffer[1],pbuffer[2],pbuffer[3]);
}
#endif


#include <interface_client.h>
using namespace android;

InterfaceClient *pClient = NULL;
DxB_ERR_CODE TCC_DxB_REMOTEPLAY_Open(void)
{
#ifdef      ENABLE_TCC_TRANSCODER
    TrscoderInfo TrsStream;
    TrsStream.bps = 1*1024*1024;
    TrsStream.fps = 25;
    TrsStream.keyframe_interval = 0;
    TrsStream.target_width = 640;
    TrsStream.target_height = 480;
    TCC_HLS_Trscoder_Init(&TrsStream);
    giStartTransCoder = 0;
    TCC_HLS_SetOutputCallback(TSOuputCallback);
	return DxB_ERR_OK;
#endif    
    if(pClient)
	    return DxB_ERR_OK;

    pClient = new InterfaceClient();
    if(pClient == NULL)
        return DxB_ERR_ERROR;
	return DxB_ERR_OK;
}

DxB_ERR_CODE TCC_DxB_REMOTEPLAY_Close(void)
{
#ifdef      ENABLE_TCC_TRANSCODER
    TCC_HLS_Trscoder_Stop();    
	return DxB_ERR_OK;
#endif        
    if(pClient)
        delete pClient;

    pClient = NULL;
	return DxB_ERR_OK;
}

DxB_ERR_CODE TCC_DxB_REMOTEPLAY_SetVideoInfo(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiFrameRate)    
{
    DxB_ERR_CODE ret = DxB_ERR_OK;

#ifdef      ENABLE_TCC_TRANSCODER
    TCC_HLS_SendVideoInfo(uiWidth, uiHeight, uiFrameRate*1000);    
	return DxB_ERR_OK;
#endif            
    if(pClient)
        pClient->WriteVideoFrameInfo(uiWidth, uiHeight, uiFrameRate*1000);
    else
        ret = DxB_ERR_ERROR;  
	return ret;
}

DxB_ERR_CODE TCC_DxB_REMOTEPLAY_WriteVideo(unsigned char *pucData, unsigned int uiSize, unsigned int uiPTS, unsigned int uiPicType)
{
    DxB_ERR_CODE ret = DxB_ERR_OK;

#ifdef      ENABLE_TCC_TRANSCODER
    if(giStartTransCoder == 0)
    {
//        TCC_HLS_Trscoder_Start();
        giStartTransCoder = 1;
    }
    TCC_HLS_WriteVideoFrame(pucData, uiSize, uiPTS, uiPicType);    
	return DxB_ERR_OK;
#endif                
    if(pClient)
        pClient->WriteVideoFrame(pucData, uiSize, uiPTS, uiPicType, NULL);
    else
        ret = DxB_ERR_ERROR;  
	return ret;
}

DxB_ERR_CODE TCC_DxB_REMOTEPLAY_WriteAudio(unsigned char *pucData, unsigned int uiSize, unsigned int uiPTS)
{
    DxB_ERR_CODE ret = DxB_ERR_OK;

#ifdef      ENABLE_TCC_TRANSCODER
    if(giStartTransCoder == 0)
    {
//        TCC_HLS_Trscoder_Start();
        giStartTransCoder = 1;
    }    
    TCC_HLS_WriteAudioFrame(pucData, uiSize, uiPTS);    
	return DxB_ERR_OK;
#endif                    
    if(pClient)
        pClient->WriteAudioFrame(pucData, uiSize, uiPTS, NULL);
    else
        ret = DxB_ERR_ERROR;  
	return ret;
}

DxB_ERR_CODE TCC_DxB_REMOTEPLAY_SetAudioInfo(unsigned int uiSampleRate, unsigned int uiChannelCounts)
{
    DxB_ERR_CODE ret = DxB_ERR_OK;
#ifdef      ENABLE_TCC_TRANSCODER
    TCC_HLS_SendAudioInfo(uiSampleRate, uiChannelCounts);    
	return DxB_ERR_OK;
#endif                        
    if(pClient)
        pClient->WriteAudioInfo(uiSampleRate, uiChannelCounts);
    else
        ret = DxB_ERR_ERROR;  
	return ret;
}

DxB_ERR_CODE TCC_DxB_REMOTEPLAY_WriteSubtitle(void *pData, unsigned int uiFormat)
{
    DxB_ERR_CODE ret = DxB_ERR_OK;
    if(pClient)
        pClient->WriteSubtitleFrame(pData, uiFormat, NULL);
    else
        ret = DxB_ERR_ERROR;  
	return ret;
}

#endif
