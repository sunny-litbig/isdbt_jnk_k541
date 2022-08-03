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
#ifndef __LINUXTV_FRONTEND_H__
#define __LINUXTV_FRONTEND_H__

#include <frontend.h>

enum {

	LINUXTV_ISDB     = 0x100,
	LINUXTV_ISDBT,

	LINUXTV_DMB      = 0x200,
	LINUXTV_TDMB,

	LINUXTV_DVB      = 0x300,
	LINUXTV_DVB_T,
	LINUXTV_DVB_T2,
	LINUXTV_DVB_S,
	LINUXTV_DVB_S2,

	LINUXTV_ATSC     = 0x500,
	LINUXTV_ATSC_D,           // atsc digital
	LINUXTV_ATSC_A,           // atsc analog
};

typedef struct linuxtv_t {
	int iTccDxbCtrl;            // handle of tcc_dxb_ctrl
	int iBoardType;             // type of board
	int iDeviceIdx;             // device index
	int iAntennaCtrl;           // mode for antenna control

	int iIndex;                 // index of frontend
	int iFrontend;              // handle of frontend
	int iSystemType;            // standard of dxb
	int iFrequency;             // frequency
	int iSymbolRate;            // symbol rate for dvbs
	int iModulation;            // modulation for atsc
	int iBandwidth;             // bandwidth for dvbt
	int iConfigChanged;         // setting of tuner is changed

	int iBlindScanChannelIndex; // scanned channel index
	int iBlindScanChannelCount; // found channel count
	int iBlindScanProgress;     // progress of blind scan

	struct dvb_frontend_info stInfo;
} linuxtv_t;

int LinuxTV_Frontend_Open(linuxtv_t *pHandle);
int LinuxTV_Frontend_Close(linuxtv_t *pHandle);
int LinuxTV_Frontend_SetProperties(linuxtv_t *pHandle);
int LinuxTV_Frontend_GetProperties(linuxtv_t *pHandle);
int LinuxTV_Frontend_GetInfo(linuxtv_t *pHandle, int *piStatus, unsigned int *puiSize);
int LinuxTV_Frontend_DiSEqC_ResetOverload(linuxtv_t *pHandle);
int LinuxTV_Frontend_DiSEqC_SendCMD(linuxtv_t *pHandle, unsigned char *msg, unsigned int len);
int LinuxTV_Frontend_DiSEqC_GetReply(linuxtv_t *pHandle, unsigned char *msg, unsigned int *len);
int LinuxTV_Frontend_DiSEqC_SendBurst(linuxtv_t *pHandle, int arg);
int LinuxTV_Frontend_SetTone(linuxtv_t *pHandle, int arg);
int LinuxTV_Frontend_SetVoltage(linuxtv_t *pHandle, int arg);
int LinuxTV_Frontend_EnableHighLNBVoltage(linuxtv_t *pHandle, int arg);
int LinuxTV_Frontend_GetStatus(linuxtv_t *pHandle, int *status);
int LinuxTV_Frontend_GetBER(linuxtv_t *pHandle, unsigned int *ber);
int LinuxTV_Frontend_GetSignalQuality(linuxtv_t *pHandle, unsigned int *quality);
int LinuxTV_Frontend_GetSignalStrength(linuxtv_t *pHandle, unsigned int *str);
int LinuxTV_Frontend_GetSNR(linuxtv_t *pHandle, unsigned int *snr);
int LinuxTV_Frontend_GetUncorrectedBlocks(linuxtv_t *pHandle, unsigned int *ucblocks);
int LinuxTV_Frontend_SetFrontend(linuxtv_t *pHandle);
int LinuxTV_Frontend_GetFrontend(linuxtv_t *pHandle);
int LinuxTV_Frontend_SetTuneMode(linuxtv_t *pHandle, unsigned int arg);
int LinuxTV_Frontend_WaitLock(linuxtv_t *pHandle, int timeout);
int LinuxTV_Frontend_DishNetwork_SendLegacyCMD(linuxtv_t *pHandle, unsigned int arg);
int LinuxTV_Frontend_SetAntennaPower(linuxtv_t *pHandle, int arg);
int LinuxTV_Frontend_BlindScan_Start(linuxtv_t *pHandle);
int LinuxTV_Frontend_BlindScan_Cancel(linuxtv_t *pHandle);
int LinuxTV_Frontend_BlindScan_GetState(linuxtv_t *pHandle, int *state);
int LinuxTV_Frontend_BlindScan_GetInfo(linuxtv_t *pHandle, int *percent, int *index, int *freqMHz, int *symKHz);
int LinuxTV_Frontend_SetDataPLP(linuxtv_t *pHandle, int arg);
int LinuxTV_Frontend_GetDataPLPs(linuxtv_t *pHandle, int *piPLPIds, int *piPLPNum);

#endif //__LINUXTV_FRONTEND_H__
