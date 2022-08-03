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
typedef void* TCC_VIDEO_UTILS;

TCC_VIDEO_UTILS TCCDxB_ScalerOpen(void);
void TCCDxB_ScalerClose(TCC_VIDEO_UTILS hUtils);
int TCCDxB_ScalerCopyData(TCC_VIDEO_UTILS hUtils, unsigned int width, unsigned int height, unsigned char *YSrc, unsigned char *USrc, unsigned char *VSrc,
								char bSrcYUVInter, unsigned char *addrDst, unsigned char ignoreAligne, int fieldInfomation, int interlaceMode);
int TCCDxB_CaptureImage(TCC_VIDEO_UTILS hUtils, unsigned int width, unsigned int height, int *yuvbuffer, int format, int interlaceMode, int fieldInfomation, unsigned char *strFilePath, int useMali);

