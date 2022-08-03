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


#ifndef __SUBTITLE_DRAW_H__
#define __SUBTITLE_DRAW_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ROUNDING_UP(x)			(int)((x)+0.9)

#define	LCD_ROT0L			0
#define	LCD_ROT90L			1
#define	LCD_ROT180L		2
#define	LCD_ROT270L		3
#define	LCD_MIR_ROT0L		10
#define	LCD_MIR_ROT90L	11
#define	LCD_MIR_ROT180L	12
#define	LCD_MIR_ROT270L	13

/* Temprary Half Intensity Color */
#define COLOR_LIGHT_RED		COLOR_RED
#define COLOR_LIGHT_GREEN		COLOR_GREEN
#define COLOR_LIGHT_YELLOW		COLOR_YELLOW
#define COLOR_LIGHT_BLUE		COLOR_BLUE
#define COLOR_LIGHT_MAGENTA	COLOR_MAGENTA
#define COLOR_LIGHT_CYAN		COLOR_CYAN
#define COLOR_LIGHT_WHITE		COLOR_WHITE


typedef struct{
	int act_x;
	int act_y;
	int font_w;
	int font_h;
	int font_hs;
	int font_vs;
	int disp_w;
	int disp_h;
	int pitch_w;
	int pitch_h;
	double ratio_x;
	double ratio_y;
	unsigned int foreColor;
	unsigned int backColor;
	unsigned short dispMode;
}T_CHAR_DISP_INFO_TYPE;

/****************************************************************************
DEFINITION OF STRUCTURE
****************************************************************************/
extern void ISDBTCap_FillBox(void *p_disp_info, unsigned int *pDst, int sx, int sy, int ex, int ey, unsigned int clr);
extern int ISDBTCap_DisplayClear(void *p_disp_info, unsigned int *pDst, int x, int y, int w, int h);
extern int ISDBTCap_DisplayUnicode(void *p_disp_info, unsigned int *pDst, int x, int y, const char *string, const char *non_string, unsigned int fclr, unsigned int bclr, unsigned short usMode);
extern void ISDBTCap_DisplayDRCS(void *p_disp_info, unsigned int *pDst, int x, int y, int width, int height, unsigned char *bitmap, unsigned short usMode);
extern int ISDBTString_MakeUnicodeString(char *pCharStr, short *pUniStr, int lang);
extern int ISDBTString_MakeUnicodeString_For_Message(char *pCharStr, short *pUniStr, int lang);


#ifdef __cplusplus
}
#endif

#endif	/* __SUBTITLE_DRAW_H__ */

