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

#ifndef     _TCC_DXB_CONTROL_H_
#define     _TCC_DXB_CONTROL_H_
#include    <linux/types.h>
typedef enum
{
	BOARD_ISDBT_TCC353X,
	BOARD_MAX
}DXB_BOARD_TYPE;

typedef struct
{
	unsigned int uiI2C; //control channel of i2c
	unsigned int uiSPI; //control channel of spi
}ST_CTRLINFO_ARG;

#define DXB_RF_PATH_UHF 1
#define DXB_RF_PATH_VHF 2

#define DXB_CTRL_DEV_FILE		"/dev/tcc_isdbt_ctrl"
#define DXB_CTRL_DEV_NAME		"tcc_isdbt_ctrl"
#define DXB_CTRL_DEV_MAJOR		250
#define DXB_CTRL_DEV_MINOR		0

#define IOCTL_DXB_CTRL_OFF		    _IO(DXB_CTRL_DEV_MAJOR, 1)
#define IOCTL_DXB_CTRL_ON			_IO(DXB_CTRL_DEV_MAJOR, 2)
#define IOCTL_DXB_CTRL_RESET    	_IO(DXB_CTRL_DEV_MAJOR, 3)
#define IOCTL_DXB_CTRL_SET_BOARD    _IO(DXB_CTRL_DEV_MAJOR, 4)
#define IOCTL_DXB_CTRL_GET_CTLINFO  _IO(DXB_CTRL_DEV_MAJOR, 5)
#define IOCTL_DXB_CTRL_RF_PATH      _IO(DXB_CTRL_DEV_MAJOR, 6)
#define IOCTL_DXB_CTRL_SET_CTRLMODE _IO(DXB_CTRL_DEV_MAJOR, 7)

#endif
