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

#ifndef __TSPARSE_ISDBT_PNGDEC_H__
#define __TSPARSE_ISDBT_PNGDEC_H__

#include <TCCXXX_PNG_DEC.h>
#include <TCCXXX_IMAGE_CUSTOM_OUTPUT_SET_ISDBT.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ConvertARGB4444toRGBA4444(a) (((a&0xf000)>>12)|((a&0x0fff)<<4))
#define ConvertARGB8888toARGB4444(a) (((a&0xf0000000)>>16)|((a&0xf00000)>>12)|((a&0xf000)>>8)|((a&0xf0)>>4))
#define ConvertARGB8888toRGBA4444(a) (((a&0xf00000)>>8)|((a&0xf000)>>4)|((a&0xf0)>>0)|((a&0xf0000000)>>28))

typedef enum{
	ISDBT_PNGDEC_ARGB4444,
	ISDBT_PNGDEC_RGBA4444,
	ISDBT_PNGDEC_ARGB8888,
	ISDBT_PNGDEC_MAX
}ISDBT_PNGDEC_FORMAT;
	
typedef struct{
	char *pBuf;
	int total_size;
	int cur_pos;
}FILE_CTX;

extern int tcc_isdbt_png_init(void *pSrc, int SrcBufSize, unsigned int *pWidth, unsigned int *pHeight);
extern int tcc_isdbt_png_dec(void *pSrc, int SrcBufSize, void *pDst, int dst_w_pitch, int png_x, int png_y, int png_w, int png_h, int format, int cc, int logo);

#ifdef __cplusplus
}
#endif

#endif	/* __TSPARSE_ISDBT_PNGDEC_H__ */	