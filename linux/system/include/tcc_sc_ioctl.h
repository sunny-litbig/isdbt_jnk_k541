/* linux/arch/arm/mach-tcc92xx/include/mach/tcc_sc_ioctl.h
 *
 * Copyright (C) 2009, 2010 Telechips, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/types.h>
#include <linux/ioctl.h>
 
#ifndef _TCC_SC_IOCTL_H_
#define _TCC_SC_IOCTL_H_

// Error Definition
#define TCC_SC_SUCCESS						0
#define TCC_SC_ERROR_UNKNOWN				-1
#define TCC_SC_ERROR_UNSUPPORT			-2
#define TCC_SC_ERROR_INVALID_STATE		-3
#define TCC_SC_ERROR_INVALID_PARAM		-4
#define TCC_SC_ERROR_INVALID_ATR			-5
#define TCC_SC_ERROR_PARITY				-6
#define TCC_SC_ERROR_SIGNAL				-7
#define TCC_SC_ERROR_TIMEOUT				-8
#define TCC_SC_ERROR_IO					-9
#define TCC_SC_ERROR_COMPLEMENT			-10
#define TCC_SC_ERROR_NACK				-11

// State Definition
enum
{
	TCC_SC_STATE_NONE = 0,
	TCC_SC_STATE_OPEN,
	TCC_SC_STATE_ENABLE,
	TCC_SC_STATE_ACTIVATE,
	TCC_SC_STATE_MAX
};

// IOCTL Command Definition
enum
{
	TCC_SC_IOCTL_SET_VCC_LEVEL = 10,
	TCC_SC_IOCTL_ENABLE,
	TCC_SC_IOCTL_ACTIVATE,
	TCC_SC_IOCTL_RESET,
	TCC_SC_IOCTL_DETECT_CARD,
	TCC_SC_IOCTL_SET_CONFIG,
	TCC_SC_IOCTL_GET_CONFIG,
	TCC_SC_IOCTL_SET_IO_PIN_CONFIG,
	TCC_SC_IOCTL_GET_IO_PIN_CONFIG,
	TCC_SC_IOCTL_SEND,
	TCC_SC_IOCTL_RECV,
	TCC_SC_IOCTL_SEND_RCV,
	TCC_SC_IOCTL_NDS_SEND_RCV,
	TCC_SC_IOCTL_MAX
};

// Voltage Level
enum
{
	TCC_SC_VOLTAGE_LEVEL_3V = 0,
	TCC_SC_VOLTAGE_LEVEL_5V,
	TCC_sC_VOLTAGE_LEVEL_MAX
};

// Direction Definition
enum
{
	TCC_SC_DIRECTION_FROM_CARD = 0,
	TCC_SC_DIRECTION_TO_CARD,
	TCC_SC_DIRECTION_MAX
};

// Protocol Definition
enum
{
	TCC_SC_PROTOCOL_T0 = 0,
	TCC_SC_PROTOCOL_T1,
	TCC_SC_PROTOCOL_MAX
};

// Direction Definition
enum
{
	TCC_SC_DIRECT_CONVENTION = 0x3b,
	TCC_SC_INVERSE_CONVENTION = 0x3f,
};

// Parity Definition
enum
{
	TCC_SC_PARITY_DISABLE = 0,
	TCC_SC_PARITY_ODD,
	TCC_SC_PARITY_EVEN,
	TCC_SC_PARITY_MAX
};

// Error Signal Definition
enum
{
	TCC_SC_ERROR_SIGNAL_DISABLE = 0,
	TCC_SC_ERROR_SIGNAL_ENABLE,
	TCC_SC_ERROR_SIGNAL_MAX
};

// Flow Control Definition for NDS
enum
{
	TCC_SC_FLOW_CONTROL_DISABLE = 0,
	TCC_SC_FLOW_CONTROL_ENABLE,
	TCC_SC_FLOW_CONTROL_MAX
};

// I/O Pin Mask Definition for NDS
enum
{
	TCC_SC_PIN_MASK_IO_C7_ON		= 0x04,
	TCC_SC_PIN_MASK_IO_C4_ON		= 0x08,
	TCC_SC_PIN_MASK_IO_C8_ON		= 0x10,
	TCC_SC_PIN_MASK_C7_ON			= 0x80,
	TCC_SC_PIN_MASK_C4_ON			= 0x20,
	TCC_SC_PIN_MASK_C8_ON			= 0x40,
	TCC_SC_PIN_MASK_MAX
};

// Configuration Parameter
typedef struct
{
	unsigned int		uiProtocol;
	unsigned int		uiConvention;
	unsigned int		uiParity;
	unsigned int		uiErrorSignal;
	unsigned int		uiFlowControl;

	unsigned int		uiClock;
	unsigned int		uiBaudrate;
	
	unsigned int		uiWaitTime;
	unsigned int		uiGuardTime;
} stTCC_SC_CONFIG;

// Send/Receive Buffer
typedef struct
{
	unsigned char		*pucTxBuf;
	unsigned int		uiTxBufLen;
	unsigned char		*pucRxBuf;
	unsigned int		*puiRxBufLen;
	unsigned int 		uiDirection;
	unsigned int		uiTimeOut;
} stTCC_SC_BUF;

#endif 
