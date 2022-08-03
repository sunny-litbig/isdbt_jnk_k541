/****************************************************************************
 *   FileName    : tcc353x_main.c
 *   Description : sample source main
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved 
 
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall 
constitute any express or implied warranty of any kind, including without limitation, any warranty 
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright 
or other third party intellectual property right. No warranty is made, express or implied, 
regarding the information's accuracy, completeness, or performance. 
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of 
or in connection with this source code or the use in the source code. 
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement 
between Telechips and Company.
*
****************************************************************************/

#include "tcc353x_common.h"
#include "tcc353x_api.h"
#include "tcc353x_monitoring.h"
#include "tcc353x_user_defines.h"
#if defined (_EXAMPLE_)
#include "TCC353X_BOOT_ISDB.h"
#endif

/*  [0] Tccspi [1] sts [2] spims */
Tcc353xRegisterConfig_t Tcc353xSingle[3] = {
	{
	 /* irqMode_0x02 */
	 0x04,			/* interrupt enable, level, active low */
	 /* irqEn_0x03 */
	 0x10,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x10,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x00, TCC353X_STREAM_THRESHOLD_SPISLV_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_SPISLV_WL, 0x10,
	 /* periConfig_0x30, periConfig_0x31 */
	 0, 0,
	 /* periConfig_0x32, periConfig_0x33 */
	 0, 0,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 TCC353X_STREAM_THRESHOLD_SPISLV_WH,
	 TCC353X_STREAM_THRESHOLD_SPISLV_WL,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
	{
	 /* irqMode_0x02 */
	 0x00,			/* interrupt disable */
	 /* irqEn_0x03 */
	 0x00,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0xF0, 0x00, 0x00,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x10,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x0F, TCC353X_STREAM_THRESHOLD_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_WL, 0x90,
	 /* periConfig_0x30, periConfig_0x31 */
	 0x21, 0x10 | TCC353X_DLR,
	 /* periConfig_0x32, periConfig_0x33 */
	 STS_POLARITY | 0x12, 0x40,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 TCC353X_STREAM_THRESHOLD_WH, TCC353X_STREAM_THRESHOLD_WL,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
	{
	 /* irqMode_0x02 */
	 0x00,			/* interrupt disable */
	 /* irqEn_0x03 */
	 0x00,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0xF0, 0x00, 0x00,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x10,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x0F, TCC353X_STREAM_THRESHOLD_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_WL, 0x90,
	 /* periConfig_0x30, periConfig_0x31 */
	 0x11, TCC353X_DLR << 2,
	 /* periConfig_0x32, periConfig_0x33 */
	 0x10, 0x00,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 TCC353X_STREAM_THRESHOLD_WH, TCC353X_STREAM_THRESHOLD_WL,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
};

/*  [0] Tccspi [1] sts [2] spims */
Tcc353xRegisterConfig_t Tcc353xDiversityMaster[3] = {
	{
	 /* irqMode_0x02 */
	 0x04,			/* interrupt enable, level, active low */
	 /* irqEn_0x03 */
	 0x10,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0x00, 0xE0, 0x3F,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x12,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x00, TCC353X_STREAM_THRESHOLD_SPISLV_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_SPISLV_WL, 0x10,
	 /* periConfig_0x30, periConfig_0x31 */
	 0, 0,
	 /* periConfig_0x32, periConfig_0x33 */
	 0, 0,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 TCC353X_STREAM_THRESHOLD_SPISLV_WH,
	 TCC353X_STREAM_THRESHOLD_SPISLV_WL,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
	{
	 /* irqMode_0x02 */
	 0x00,			/* interrupt disable */
	 /* irqEn_0x03 */
	 0x00,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0xF0, 0xE0, 0x3F,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x12,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x0F, TCC353X_STREAM_THRESHOLD_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_WL, 0x90,
	 /* periConfig_0x30, periConfig_0x31 */
	 0x21, TCC353X_DLR,
	 /* periConfig_0x32, periConfig_0x33 */
	 STS_POLARITY | 0x12, 0x40,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 TCC353X_STREAM_THRESHOLD_WH, TCC353X_STREAM_THRESHOLD_WL,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
	{
	 /* irqMode_0x02 */
	 0x00,			/* interrupt disable */
	 /* irqEn_0x03 */
	 0x00,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0xF0, 0xE0, 0x3F,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 TCC353X_DRV_STR_GPIO_0x13_07_00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x12,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x0F, TCC353X_STREAM_THRESHOLD_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_WL, 0x90,
	 /* periConfig_0x30, periConfig_0x31 */
	 0x11, TCC353X_DLR << 2,
	 /* periConfig_0x32, periConfig_0x33 */
	 0x10, 0x00,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 TCC353X_STREAM_THRESHOLD_WH, TCC353X_STREAM_THRESHOLD_WL,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
};

/*  [0] Tccspi [1] sts [2] spims */
Tcc353xRegisterConfig_t Tcc353xDiversitySlaveMid[3] = {
	{
	 /* irqMode_0x02 */
	 0x04,			/* interrupt enable, level, active low */
	 /* irqEn_0x03 */
	 0x10,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0xF0, 0xFF, 0x3F,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x13,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x00, TCC353X_STREAM_THRESHOLD_SPISLV_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_SPISLV_WL, 0x10,
	 /* periConfig_0x30, periConfig_0x31 */
	 0, 0,
	 /* periConfig_0x32, periConfig_0x33 */
	 0, 0,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 0, 0,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
	{
	 /* irqMode_0x02 */
	 0x00,			/* interrupt disable */
	 /* irqEn_0x03 */
	 0x00,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0xF0, 0xFF, 0x3F,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x13,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x0F, TCC353X_STREAM_THRESHOLD_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_WL, 0x90,
	 /* periConfig_0x30, periConfig_0x31 */
	 0, 0,
	 /* periConfig_0x32, periConfig_0x33 */
	 0, 0,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 0, 0,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
	{
	 /* irqMode_0x02 */
	 0x00,			/* interrupt disable */
	 /* irqEn_0x03 */
	 0x00,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0xF0, 0xFF, 0x3F,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x13,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x0F, TCC353X_STREAM_THRESHOLD_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_WL, 0x90,
	 /* periConfig_0x30, periConfig_0x31 */
	 0, 0,
	 /* periConfig_0x32, periConfig_0x33 */
	 0, 0,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 0, 0,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
};

/*  [0] Tccspi [1] sts [2] spims */
Tcc353xRegisterConfig_t Tcc353xDiversitySlaveLast[3] = {
	{
	 /* irqMode_0x02 */
	 0x04,			/* interrupt enable, level, active low */
	 /* irqEn_0x03 */
	 0x10,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0xF0, 0x3F, 0x00,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x11,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x00, TCC353X_STREAM_THRESHOLD_SPISLV_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_SPISLV_WL, 0x10,
	 /* periConfig_0x30, periConfig_0x31 */
	 0, 0,
	 /* periConfig_0x32, periConfig_0x33 */
	 0, 0,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 0, 0,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
	{
	 /* irqMode_0x02 */
	 0x00,			/* interrupt disable */
	 /* irqEn_0x03 */
	 0x00,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0xF0, 0x3F, 0x00,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x11,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x0F, TCC353X_STREAM_THRESHOLD_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_WL, 0x90,
	 /* periConfig_0x30, periConfig_0x31 */
	 0, 0,
	 /* periConfig_0x32, periConfig_0x33 */
	 0, 0,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 0, 0,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
	{
	 /* irqMode_0x02 */
	 0x00,			/* interrupt disable */
	 /* irqEn_0x03 */
	 0x00,
	 /* initRemap_0x0D, initPC_0x0E,  initPC_0x0F */
	 TCC353X_REMAP_TYPE, TCC353X_INIT_PC_H, TCC353X_INIT_PC_L,
	 /* gpioAlt_0x10_07_00, gpioAlt_0x10_15_08, gpioAlt_0x10_23_16 */
	 0xF0, 0x3F, 0x00,
	 /* gpioDr_0x11_07_00, gpioDr_0x11_15_08, gpioDr_0x11_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioLr_0x12_07_00, gpioLr_0x12_15_08, gpioLr_0x12_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioDrv_0x13_07_00, gpioDrv_0x13_15_08, gpioDrv_0x13_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioPe_0x14_07_00, gpioPe_0x14_15_08, gpioPe_0x14_23_16 */
	 0x00, 0x00, 0x00,
	 /* gpioSDrv_0x15_07_00, gpioSDrv_0x15_15_08, gpioSDrv_0x15_23_16 */
	 0x00, 0x00, 0x00,
	 /*ioMisc_0x16 */
	 0x11,
	 /* streamDataConfig_0x1B, streamDataConfig_0x1C */
	 0x0F, TCC353X_STREAM_THRESHOLD_WH,
	 /* streamDataConfig_0x1D,  streamDataConfig_0x1E */
	 TCC353X_STREAM_THRESHOLD_WL, 0x90,
	 /* periConfig_0x30, periConfig_0x31 */
	 0, 0,
	 /* periConfig_0x32, periConfig_0x33 */
	 0, 0,
	 /* bufferConfig_0x4E,bufferConfig_0x4F */
	 0x11, 0x0F,
	 /* bufferConfig_0x54,bufferConfig_0x55 */
	 0, 0,
	 /* bufferConfig_0x50,bufferConfig_0x51 */
	 ((TCC353X_BUFF_A_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_START >> 2) & 0xFF),
	 /* bufferConfig_0x52,bufferConfig_0x53 */
	 ((TCC353X_BUFF_A_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_A_END >> 2) & 0xFF),
	 /* bufferConfig_0x58,bufferConfig_0x59 */
	 ((TCC353X_BUFF_B_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_START >> 2) & 0xFF),
	 /* bufferConfig_0x5A,bufferConfig_0x5B */
	 ((TCC353X_BUFF_B_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_B_END >> 2) & 0xFF),
	 /* bufferConfig_0x60,bufferConfig_0x61 */
	 ((TCC353X_BUFF_C_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_START >> 2) & 0xFF),
	 /* bufferConfig_0x62,bufferConfig_0x63 */
	 ((TCC353X_BUFF_C_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_C_END >> 2) & 0xFF),
	 /* bufferConfig_0x68,bufferConfig_0x69 */
	 ((TCC353X_BUFF_D_START >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_START >> 2) & 0xFF),
	 /* bufferConfig_0x6A,bufferConfig_0x6B */
	 ((TCC353X_BUFF_D_END >> 10) & 0xFF),
	 ((TCC353X_BUFF_D_END >> 2) & 0xFF),
	 }
	,
};

Tcc353xOption_t Tcc353xOptionSingle = {
	/* Baseband name            */
	BB_TCC3531,

	/* board type               */
	TCC353X_BOARD_SINGLE,

	/* select command interface */
	TCC353X_IF_I2C,

	/* select stream interface  */
	TCC353X_STREAM_IO_STS,

	/* current device address   */
	/* 0xA8    (first)          */
	/* 0xAA    (second)         */
	/* 0xAC    (third)          */
	/* 0xAE    (fourth)         */
	0xA8,

	/* pll option               */
	0x00, /* use default pll */ /* 0xA214 */

	/* osc clk                  */
	38400,

	/* diversity position option */
	TCC353X_DIVERSITY_NONE,

	/* Interrupt usage option (tccspi-only) */
	0,

	/* RF Switching GPIO_N */
	/* 0 : not use, N : use GPIO_N 	*/
	/* example use gpio [23 ,09 , 03, 04] => value is [0x80 02 18]*/
	(1<<GPIO_NUM_RF_SWITCHING_TCC3531_SINGLE),

	/* RF Switching value of GPIO_N  */
	/* control gpioN - 0 : off, 1: on */
	0x00, /* rfSwitchingVhfLow */
	0x00, /* rfSwitchingVhfHigh */
	(1<<GPIO_NUM_RF_SWITCHING_TCC3531_SINGLE), /* rfSwitchingUhf */
	0x00, /* rfSwitchingReserved */

	/* rf type - EnumTcc353xRfType */
	TCC353X_DUAL_BAND_RF,

	/* register config          */
	&Tcc353xSingle[1]
};

Tcc353xOption_t Tcc353xOption2Diversity[2] = {
	{
	 /* Baseband name            */
	 BB_TCC3531,

	 /* board type               */
	 TCC353X_BOARD_2DIVERSITY,

	 /* select command interface */
	 TCC353X_IF_I2C,

	 /* select stream interface  */
	 TCC353X_STREAM_IO_STS,

	 /* current device address   */
	 /* 0xA8    (first)          */
	 /* 0xAA    (second)         */
	 /* 0xAC    (third)          */
	 /* 0xAE    (fourth)         */
	 0xA8,

	 /* pll option               */
	 0x00, /* use default pll */ /* 0xA214 */

	 /* osc clk                  */
	 38400,

	 /* diversity position option */
	 TCC353X_DIVERSITY_MASTER,

	 /* Interrupt usage option (tccspi-only) */
	 0,

	 /* RF Switching GPIO_N */
	 /* 0 : not use, N : use GPIO_N  */
	 /* example use gpio [23 ,09 , 03, 04] => value is [0x80 02 18]*/
	 (1<<GPIO_NUM_RF_SWITCHING_TCC3531_MASTER),
	 
	 /* RF Switching value of GPIO_N  */
	 /* control gpioN - 0 : off, 1: on */
	 0x00, /* rfSwitchingVhfLow */
	 0x00, /* rfSwitchingVhfHigh */
	 (1<<GPIO_NUM_RF_SWITCHING_TCC3531_MASTER), /* rfSwitchingUhf */
	 0x00, /* rfSwitchingReserved */
	 
	 /* rf type - EnumTcc353xRfType */
	 TCC353X_DUAL_BAND_RF,
	 
	 /* register config          */
	 &Tcc353xDiversityMaster[1]
	 }
	, {
	   /* Baseband name            */
	   BB_TCC3531,

	   /* board type               */
	   TCC353X_BOARD_2DIVERSITY,
	   /* select command interface */
	   TCC353X_IF_I2C,
	   /* select stream interface  */
	   TCC353X_STREAM_IO_STS,
	   /* current device address   */
	   /* 0xA8    (first)          */
	   /* 0xAA    (second)         */
	   /* 0xAC    (third)          */
	   /* 0xAE    (fourth)         */
	   0xAA,
	   /* pll option               */
	   0x00, /* use default pll */ /* 0xA214 */
	   /* osc clk                  */
	   38400,
	   /* diversity position option */
	   TCC353X_DIVERSITY_LAST,
	   /* Interrupt usage option (tccspi-only) */
	   0,

	   /* RF Switching GPIO_N */
	   /* 0 : not use, N : use GPIO_N  */
	   /* example use gpio [23 ,09 , 03, 04] => value is [0x80 02 18]*/
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE),
	   
	   /* RF Switching value of GPIO_N  */
	   /* control gpioN - 0 : off, 1: on */
	   0x00, /* rfSwitchingVhfLow */
	   0x00, /* rfSwitchingVhfHigh */
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE), /* rfSwitchingUhf */
	   0x00, /* rfSwitchingReserved */
	   
	   /* rf type - EnumTcc353xRfType */
	   TCC353X_DUAL_BAND_RF,
		   
	   /* register config          */
	   &Tcc353xDiversitySlaveLast[1]
	   }
};

Tcc353xOption_t Tcc353xOption3Diversity[3] = {
	{
	 /* Baseband name            */
	 BB_TCC3531,

	 /* board type               */
	 TCC353X_BOARD_3DIVERSITY,

	 /* select command interface */
	 TCC353X_IF_I2C,

	 /* select stream interface  */
	 TCC353X_STREAM_IO_STS,

	 /* current device address   */
	 /* 0xA8    (first)          */
	 /* 0xAA    (second)         */
	 /* 0xAC    (third)          */
	 /* 0xAE    (fourth)         */
	 0xA8,

	 /* pll option               */
	 0x00, /* use default pll */ /* 0xA214 */

	 /* osc clk                  */
	 38400,

	 /* diversity position option */
	 TCC353X_DIVERSITY_MASTER,

	 /* Interrupt usage option (tccspi-only) */
	 0,

	 /* RF Switching GPIO_N */
	 /* 0 : not use, N : use GPIO_N  */
	 /* example use gpio [23 ,09 , 03, 04] => value is [0x80 02 18]*/
	 (1<<GPIO_NUM_RF_SWITCHING_TCC3531_MASTER),
	 
	 /* RF Switching value of GPIO_N  */
	 /* control gpioN - 0 : off, 1: on */
	 0x00, /* rfSwitchingVhfLow */
	 0x00, /* rfSwitchingVhfHigh */
	 (1<<GPIO_NUM_RF_SWITCHING_TCC3531_MASTER), /* rfSwitchingUhf */
	 0x00, /* rfSwitchingReserved */
	 
	 /* rf type - EnumTcc353xRfType */
	 TCC353X_DUAL_BAND_RF,
	 
	 /* register config          */
	 &Tcc353xDiversityMaster[1]
	 }
	, {
	   /* Baseband name            */
	   BB_TCC3531,

	   /* board type               */
	   TCC353X_BOARD_3DIVERSITY,
	   /* select command interface */
	   TCC353X_IF_I2C,
	   /* select stream interface  */
	   TCC353X_STREAM_IO_STS,
	   /* current device address   */
	   /* 0xA8    (first)          */
	   /* 0xAA    (second)         */
	   /* 0xAC    (third)          */
	   /* 0xAE    (fourth)         */
	   0xAA,
	   /* pll option               */
	   0x00, /* use default pll */ /* 0xA214 */
	   /* osc clk                  */
	   38400,
	   /* diversity position option */
	   TCC353X_DIVERSITY_MID,
	   /* Interrupt usage option (tccspi-only) */
	   0,

	   /* RF Switching GPIO_N */
	   /* 0 : not use, N : use GPIO_N  */
	   /* example use gpio [23 ,09 , 03, 04] => value is [0x80 02 18]*/
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE_2_X),
	   
	   /* RF Switching value of GPIO_N  */
	   /* control gpioN - 0 : off, 1: on */
	   0x00, /* rfSwitchingVhfLow */
	   0x00, /* rfSwitchingVhfHigh */
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE_2_X), /* rfSwitchingUhf */
	   0x00, /* rfSwitchingReserved */
	   
	   /* rf type - EnumTcc353xRfType */
	   TCC353X_DUAL_BAND_RF,

	   /* register config          */
	   &Tcc353xDiversitySlaveMid[1]
	   }
	, {
	   /* Baseband name            */
	   BB_TCC3531,

	   /* board type               */
	   TCC353X_BOARD_3DIVERSITY,
	   /* select command interface */
	   TCC353X_IF_I2C,
	   /* select stream interface  */
	   TCC353X_STREAM_IO_STS,
	   /* current device address   */
	   /* 0xA8    (first)          */
	   /* 0xAA    (second)         */
	   /* 0xAC    (third)          */
	   /* 0xAE    (fourth)         */
	   0xAC,
	   /* pll option               */
	   0x00, /* use default pll */ /* 0xA214 */
	   /* osc clk                  */
	   38400,
	   /* diversity position option */
	   TCC353X_DIVERSITY_LAST,
	   /* Interrupt usage option (tccspi-only) */
	   0,

	   /* RF Switching GPIO_N */
	   /* 0 : not use, N : use GPIO_N  */
	   /* example use gpio [23 ,09 , 03, 04] => value is [0x80 02 18]*/
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE),
	   
	   /* RF Switching value of GPIO_N  */
	   /* control gpioN - 0 : off, 1: on */
	   0x00, /* rfSwitchingVhfLow */
	   0x00, /* rfSwitchingVhfHigh */
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE), /* rfSwitchingUhf */
	   0x00, /* rfSwitchingReserved */
	   
	   /* rf type - EnumTcc353xRfType */
	   TCC353X_DUAL_BAND_RF,

	   /* register config          */
	   &Tcc353xDiversitySlaveLast[1]
	   }
};


Tcc353xOption_t Tcc353xOption4Diversity[4] = {
	{
	 /* Baseband name            */
	 BB_TCC3531,

	 /* board type               */
	 TCC353X_BOARD_4DIVERSITY,

	 /* select command interface */
	 TCC353X_IF_I2C,

	 /* select stream interface  */
	 TCC353X_STREAM_IO_STS,

	 /* current device address   */
	 /* 0xA8    (first)          */
	 /* 0xAA    (second)         */
	 /* 0xAC    (third)          */
	 /* 0xAE    (fourth)         */
	 0xA8,

	 /* pll option               */
	 0x00, /* use default pll */ /* 0xA214 */

	 /* osc clk                  */
	 38400,

	 /* diversity position option */
	 TCC353X_DIVERSITY_MASTER,

	 /* Interrupt usage option (tccspi-only) */
	 0,

	 /* RF Switching GPIO_N */
	 /* 0 : not use, N : use GPIO_N  */
	 /* example use gpio [23 ,09 , 03, 04] => value is [0x80 02 18]*/
	 (1<<GPIO_NUM_RF_SWITCHING_TCC3531_MASTER),
	 
	 /* RF Switching value of GPIO_N  */
	 /* control gpioN - 0 : off, 1: on */
	 0x00, /* rfSwitchingVhfLow */
	 0x00, /* rfSwitchingVhfHigh */
	 (1<<GPIO_NUM_RF_SWITCHING_TCC3531_MASTER), /* rfSwitchingUhf */
	 0x00, /* rfSwitchingReserved */
	 
	 /* rf type - EnumTcc353xRfType */
	 TCC353X_DUAL_BAND_RF,
	 
	 /* register config          */
	 &Tcc353xDiversityMaster[1]
	 }
	, {
	   /* Baseband name            */
	   BB_TCC3531,

	   /* board type               */
	   TCC353X_BOARD_4DIVERSITY,
	   /* select command interface */
	   TCC353X_IF_I2C,
	   /* select stream interface  */
	   TCC353X_STREAM_IO_STS,
	   /* current device address   */
	   /* 0xA8    (first)          */
	   /* 0xAA    (second)         */
	   /* 0xAC    (third)          */
	   /* 0xAE    (fourth)         */
	   0xAA,
	   /* pll option               */
	   0x00, /* use default pll */ /* 0xA214 */
	   /* osc clk                  */
	   38400,
	   /* diversity position option */
	   TCC353X_DIVERSITY_MID,
	   /* Interrupt usage option (tccspi-only) */
	   0,

	   /* RF Switching GPIO_N */
	   /* 0 : not use, N : use GPIO_N  */
	   /* example use gpio [23 ,09 , 03, 04] => value is [0x80 02 18]*/
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE_2_X),
	   
	   /* RF Switching value of GPIO_N  */
	   /* control gpioN - 0 : off, 1: on */
	   0x00, /* rfSwitchingVhfLow */
	   0x00, /* rfSwitchingVhfHigh */
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE_2_X), /* rfSwitchingUhf */
	   0x00, /* rfSwitchingReserved */
	   
	   /* rf type - EnumTcc353xRfType */
	   TCC353X_DUAL_BAND_RF,

	   /* register config          */
	   &Tcc353xDiversitySlaveMid[1]
	   }
	, {
	   /* Baseband name            */
	   BB_TCC3531,

	   /* board type               */
	   TCC353X_BOARD_4DIVERSITY,
	   /* select command interface */
	   TCC353X_IF_I2C,
	   /* select stream interface  */
	   TCC353X_STREAM_IO_STS,
	   /* current device address   */
	   /* 0xA8    (first)          */
	   /* 0xAA    (second)         */
	   /* 0xAC    (third)          */
	   /* 0xAE    (fourth)         */
	   0xAC,
	   /* pll option               */
	   0x00, /* use default pll */ /* 0xA214 */
	   /* osc clk                  */
	   38400,
	   /* diversity position option */
	   TCC353X_DIVERSITY_MID,
	   /* Interrupt usage option (tccspi-only) */
	   0,

	   /* RF Switching GPIO_N */
	   /* 0 : not use, N : use GPIO_N  */
	   /* example use gpio [23 ,09 , 03, 04] => value is [0x80 02 18]*/
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE_3_X),
	   
	   /* RF Switching value of GPIO_N  */
	   /* control gpioN - 0 : off, 1: on */
	   0x00, /* rfSwitchingVhfLow */
	   0x00, /* rfSwitchingVhfHigh */
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE_3_X), /* rfSwitchingUhf */
	   0x00, /* rfSwitchingReserved */
	   
	   /* rf type - EnumTcc353xRfType */
	   TCC353X_DUAL_BAND_RF,

	   /* register config          */
	   &Tcc353xDiversitySlaveMid[1]
	   }
	, {
	   /* Baseband name            */
	   BB_TCC3531,

	   /* board type               */
	   TCC353X_BOARD_4DIVERSITY,
	   /* select command interface */
	   TCC353X_IF_I2C,
	   /* select stream interface  */
	   TCC353X_STREAM_IO_STS,
	   /* current device address   */
	   /* 0xA8    (first)          */
	   /* 0xAA    (second)         */
	   /* 0xAC    (third)          */
	   /* 0xAE    (fourth)         */
	   0xAE,
	   /* pll option               */
	   0x00, /* use default pll */ /* 0xA214 */
	   /* osc clk                  */
	   38400,
	   /* diversity position option */
	   TCC353X_DIVERSITY_LAST,
	   /* Interrupt usage option (tccspi-only) */
	   0,

	   /* RF Switching GPIO_N */
	   /* 0 : not use, N : use GPIO_N  */
	   /* example use gpio [23 ,09 , 03, 04] => value is [0x80 02 18]*/
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE),
	   
	   /* RF Switching value of GPIO_N  */
	   /* control gpioN - 0 : off, 1: on */
	   0x00, /* rfSwitchingVhfLow */
	   0x00, /* rfSwitchingVhfHigh */
	   (1<<GPIO_NUM_RF_SWITCHING_TCC3531_SLAVE), /* rfSwitchingUhf */
	   0x00, /* rfSwitchingReserved */
	   
	   /* rf type - EnumTcc353xRfType */
	   TCC353X_DUAL_BAND_RF,

	   /* register config          */
	   &Tcc353xDiversitySlaveLast[1]
	   }
};

