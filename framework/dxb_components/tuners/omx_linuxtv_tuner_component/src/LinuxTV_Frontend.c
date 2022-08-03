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
#define LOG_TAG	"LINUXTV_FRONTEND"

#include <utils/Log.h>
#include <cutils/properties.h>
#include <stdlib.h>

#include <sys/poll.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <LinuxTV_Frontend.h>

//#define DBG_MSG
#ifdef DBG_MSG
#define DEBUG_MSG(msg...)	ALOGD(msg)
#else
#define DEBUG_MSG(msg...)
#endif

static int linuxtv_fe_open(linuxtv_t *pHandle)
{
	char fedev[32];

	if (pHandle->iFrontend >= 0)
		return -1;

	sprintf(fedev, "/dev/dvb%d.frontend0", pHandle->iIndex);

	pHandle->iFrontend = open(fedev, O_RDWR | O_NONBLOCK);

	return (pHandle->iFrontend >= 0) ? 0 : -1;
}

static int linuxtv_fe_close(linuxtv_t *pHandle)
{
	if (pHandle->iFrontend < 0)
		return -1;

	close(pHandle->iFrontend);

	pHandle->iFrontend = -1;

	return 0;
}

static int linuxtv_fe_ioctl_get_info(linuxtv_t *pHandle, struct dvb_frontend_info *pInfo)
{
	if (pHandle->iFrontend < 0)
		return -1;

	memset(pInfo, 0x0, sizeof(struct dvb_frontend_info));

	return ioctl(pHandle->iFrontend, FE_GET_INFO, pInfo);
}

static int linuxtv_fe_ioctl_set_properties(linuxtv_t *pHandle, struct dtv_properties *pTvps)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_SET_PROPERTY, pTvps);
}

static int linuxtv_fe_ioctl_get_properties(linuxtv_t *pHandle, struct dtv_properties *pTvps)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_GET_PROPERTY, pTvps);
}

static int linuxtv_fe_ioctl_diseqc_reset_overload(linuxtv_t *pHandle)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_DISEQC_RESET_OVERLOAD, 0);
}

static int linuxtv_fe_ioctl_diseqc_send_master_cmd(linuxtv_t *pHandle, struct dvb_diseqc_master_cmd *pCMD)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_DISEQC_SEND_MASTER_CMD, pCMD);
}

static int linuxtv_fe_ioctl_diseqc_recv_slave_reply(linuxtv_t *pHandle, struct dvb_diseqc_slave_reply *pReply)
{
	if (pHandle->iFrontend < 0)
		return -1;

	memset(pReply, 0x0, sizeof(struct dvb_diseqc_slave_reply));

	return ioctl(pHandle->iFrontend, FE_DISEQC_RECV_SLAVE_REPLY, pReply);
}

static int linuxtv_fe_ioctl_diseqc_send_burst(linuxtv_t *pHandle, fe_sec_mini_cmd_t cmd)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_DISEQC_SEND_BURST, cmd);
}

static int linuxtv_fe_ioctl_set_tone(linuxtv_t *pHandle, fe_sec_tone_mode_t mode)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_SET_TONE, mode);
}

static int linuxtv_fe_ioctl_set_voltage(linuxtv_t *pHandle, fe_sec_voltage_t voltage)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_SET_VOLTAGE, voltage);
}

static int linuxtv_fe_ioctl_enable_high_lnb_voltage(linuxtv_t *pHandle, int iARG)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_ENABLE_HIGH_LNB_VOLTAGE, iARG);
}

static int linuxtv_fe_ioctl_read_status(linuxtv_t *pHandle, fe_status_t *pStatus)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_READ_STATUS, pStatus);
}

static int linuxtv_fe_ioctl_read_ber(linuxtv_t *pHandle, unsigned int *puiBER)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_READ_BER, puiBER);
}

static int linuxtv_fe_ioctl_read_signal_strength(linuxtv_t *pHandle, unsigned short *pusSTR)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_READ_SIGNAL_STRENGTH, pusSTR);
}

static int linuxtv_fe_ioctl_read_snr(linuxtv_t *pHandle, unsigned short *pusSNR)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_READ_SNR, pusSNR);
}

static int linuxtv_fe_ioctl_read_uncorrected_blocks(linuxtv_t *pHandle, unsigned int *puiBlocks)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_READ_UNCORRECTED_BLOCKS, puiBlocks);
}

static int linuxtv_fe_ioctl_set_frontend(linuxtv_t *pHandle, struct dvb_frontend_parameters *pParams)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_SET_FRONTEND, pParams);
}

static int linuxtv_fe_ioctl_get_frontend(linuxtv_t *pHandle, struct dvb_frontend_parameters *pParams)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_GET_FRONTEND, pParams);
}

static int linuxtv_fe_ioctl_set_frontend_tune_mode(linuxtv_t *pHandle, unsigned int uiARG)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_SET_FRONTEND_TUNE_MODE, uiARG);
}

static int linuxtv_fe_ioctl_get_event(linuxtv_t *pHandle, struct dvb_frontend_event *pEvent, int iWaitTime)
{
	struct pollfd pfd[1];

	if (pHandle->iFrontend < 0)
		return -1;

	if (iWaitTime > 0)
	{
		pfd[0].fd = pHandle->iFrontend;
		pfd[0].events = POLLIN;

		if (poll(pfd, 1, iWaitTime) == 0 || (pfd[0].revents & POLLIN) == 0)
		{
			pEvent->status = 0;
			return 0;
		}
	}

	return ioctl(pHandle->iFrontend, FE_GET_EVENT, pEvent);
}

static int linuxtv_fe_ioctl_dishnetwork_send_legacy_cmd(linuxtv_t *pHandle, unsigned int uiARG)
{
	if (pHandle->iFrontend < 0)
		return -1;

	return ioctl(pHandle->iFrontend, FE_DISHNETWORK_SEND_LEGACY_CMD, uiARG);
}

static int linuxtv_fe_tvps_set(struct dtv_property *pTvp, unsigned int uiCMD, unsigned int uiData)
{
	memset(pTvp, 0x0, sizeof(struct dtv_property));

	pTvp->cmd = uiCMD;
	pTvp->u.data = uiData;

	return 0;
}

static int linuxtv_fe_tvps_set_ex(struct dtv_property *pTvp, unsigned int uiCMD, void *pData, unsigned int uiLen)
{
	memset(pTvp, 0x0, sizeof(struct dtv_property));

	pTvp->cmd = uiCMD;
	pTvp->u.buffer.len = uiLen;
	pTvp->u.buffer.reserved2 = pData;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int LinuxTV_Frontend_Open(linuxtv_t *pHandle)
{
	if (linuxtv_fe_open(pHandle) != 0)
	{
		ALOGE("Failed to open frontend%d", pHandle->iIndex);
		return -1;
	}

	if (linuxtv_fe_ioctl_get_info(pHandle, &pHandle->stInfo) != 0)
	{
		ALOGE("Failed to get info of frontend%d", pHandle->iIndex);
		return -1;
	}

	ALOGI("Name        : %s", pHandle->stInfo.name);
	ALOGI("Type        : %d", pHandle->stInfo.type);
	ALOGI("Frequency   : %d ~ %d (Tolerance:%d, Step:%d)",  pHandle->stInfo.frequency_min,
															pHandle->stInfo.frequency_max,
															pHandle->stInfo.frequency_tolerance,
															pHandle->stInfo.frequency_stepsize);
	ALOGI("Symbol Rate : %d ~ %d (Tolerance:%d)",   pHandle->stInfo.symbol_rate_min,
													pHandle->stInfo.symbol_rate_max,
													pHandle->stInfo.symbol_rate_tolerance);
	ALOGI("Noti Delay  : %d", pHandle->stInfo.notifier_delay);
	ALOGI("Caps        : %d", pHandle->stInfo.caps);

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_Close(linuxtv_t *pHandle)
{
	if (linuxtv_fe_close(pHandle) != 0)
	{
		ALOGE("Failed to close frontend%d", pHandle->iIndex);
		return -1;
	}
	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_GetInfo(linuxtv_t *pHandle, int *piStatus, unsigned int *puiSize)
{
	struct dtv_properties tvps;
	struct dtv_property *tvp;
	unsigned int i;
	int *p = piStatus;

	tvps.num = 9;
	tvps.props = malloc(sizeof(struct dtv_property) * tvps.num);

	tvp = tvps.props;

	linuxtv_fe_tvps_set(tvp++, DTV_FREQUENCY,           0);   // reflash tuner information
	linuxtv_fe_tvps_set(tvp++, DTV_INVERSION,           0);   // Rotation
	linuxtv_fe_tvps_set(tvp++, DTV_MODULATION,          0);   // Constellation
	linuxtv_fe_tvps_set(tvp++, DTV_INNER_FEC,           0);   // FEC Type
	linuxtv_fe_tvps_set(tvp++, DTV_GUARD_INTERVAL,      0);   // Guard Interval
	linuxtv_fe_tvps_set(tvp++, DTV_CODE_RATE_HP,        0);   // Code Rate
	linuxtv_fe_tvps_set(tvp++, DTV_TRANSMISSION_MODE,   0);   // FFT
	linuxtv_fe_tvps_set(tvp++, DTV_DVBT2_PLP_ID_LEGACY, 0);   // PLP Number
	linuxtv_fe_tvps_set(tvp++, DTV_PILOT,               0);   // PilotPP

	if (linuxtv_fe_ioctl_get_properties(pHandle, &tvps) != 0)
	{
		free(tvps.props);
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	i = 1;

	// System
	if (pHandle->iSystemType == LINUXTV_DVB_T)
		*p = 0;
	else if (pHandle->iSystemType == LINUXTV_DVB_T2)
		*p = 1;
	else
		*p = -1;

	ALOGI("Bandwidth : %d", *p);
	p++;

	// Frequency
	*p = pHandle->iFrequency;

	ALOGI("Frequency : %d", *p);
	p++;

	// Bandwidth or Symbol Rate
	if (pHandle->iSystemType == LINUXTV_DVB_S || pHandle->iSystemType == LINUXTV_DVB_S2)
		*p = pHandle->iSymbolRate;
	else
		*p = pHandle->iBandwidth;

	ALOGI("Bandwidth : %d", *p);
	p++;

	// Rotation
	if (pHandle->iSystemType != LINUXTV_DVB_T2)
		*p = -1;
	else if (tvps.props[i].u.data == INVERSION_ON)
		*p = 1;
	else
		*p = 0;

	ALOGI("Rotation : %d", *p);
	p++; i++;

	// Constellation
	if (tvps.props[i].u.data == QPSK)
		*p = 0;
	else if (tvps.props[i].u.data == QAM_16)
		*p = 1;
	else if (tvps.props[i].u.data == QAM_64)
		*p = 2;
	else if (tvps.props[i].u.data == QAM_128)
		*p = 3;
	else
		*p = -1;

	ALOGI("Constellation : %d", *p);
	p++; i++;

	// FECLength
	if (pHandle->iSystemType != LINUXTV_DVB_T2)
		*p = -1;
	else if (tvps.props[i].u.data == FEC_NONE)
		*p = 0;
	else
		*p = 1;

	ALOGI("FECLength : %d", *p);
	p++; i++;

	// GuardInterval
	if (tvps.props[i].u.data == GUARD_INTERVAL_1_32)
		*p = 0;
	else if (tvps.props[i].u.data == GUARD_INTERVAL_1_16)
		*p = 1;
	else if (tvps.props[i].u.data == GUARD_INTERVAL_1_8)
		*p = 2;
	else if (tvps.props[i].u.data == GUARD_INTERVAL_1_4)
		*p = 3;
	else if (tvps.props[i].u.data == GUARD_INTERVAL_1_128)
		*p = 4;
	else if (tvps.props[i].u.data == GUARD_INTERVAL_19_128)
		*p = 5;
	else if (tvps.props[i].u.data == GUARD_INTERVAL_19_256)
		*p = 6;
	else
		*p = -1;

	ALOGI("GuardInterval : %d", *p);
	p++; i++;

	// CodeRate
	if (tvps.props[i].u.data == FEC_1_2)
		*p = 0;
	else if (tvps.props[i].u.data == FEC_2_3)
		*p = 1;
	else if (tvps.props[i].u.data == FEC_3_4)
		*p = 2;
	else if (tvps.props[i].u.data == FEC_4_5)
		*p = 3;
	else if (tvps.props[i].u.data == FEC_5_6)
		*p = 4;
	else if (tvps.props[i].u.data == FEC_6_7)
		*p = 5;
	else if (tvps.props[i].u.data == FEC_7_8)
		*p = 6;
	else if (tvps.props[i].u.data == FEC_8_9)
		*p = 7;
	else if (tvps.props[i].u.data == FEC_3_5)
		*p = 8;
	else if (tvps.props[i].u.data == FEC_9_10)
		*p = 9;
	else
		*p = -1;

	ALOGI("CodeRate : %d", *p);
	p++; i++;

	// FFT
	if (tvps.props[i].u.data == TRANSMISSION_MODE_1K)
		*p = 0;
	else if (tvps.props[i].u.data == TRANSMISSION_MODE_2K)
		*p = 1;
	else if (tvps.props[i].u.data == TRANSMISSION_MODE_4K)
		*p = 2;
	else if (tvps.props[i].u.data == TRANSMISSION_MODE_8K)
		*p = 3;
	else if (tvps.props[i].u.data == TRANSMISSION_MODE_16K)
		*p = 4;
	else if (tvps.props[i].u.data == TRANSMISSION_MODE_32K)
		*p = 5;
	else
		*p = -1;

	ALOGI("FFT : %d", *p);
	p++; i++;

	// PLPNum
	if (pHandle->iSystemType != LINUXTV_DVB_T2)
		*p = -1;
	else
		*p = tvps.props[i].u.data;

	ALOGI("PLPNum : %d", *p);
	p++; i++;

	// PilotPP
	if (pHandle->iSystemType != LINUXTV_DVB_T2)
		*p = -1;
	else
		*p = tvps.props[i].u.buffer.reserved1[0];

	ALOGI("PilotPP : %d", *p);
	p++; i++;

	*puiSize = (p - piStatus) * (sizeof(int));

	ALOGI("Size : %d", *puiSize);

	free(tvps.props);

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_DiSEqC_ResetOverload(linuxtv_t *pHandle)
{
	if (linuxtv_fe_ioctl_diseqc_reset_overload(pHandle) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_DiSEqC_SendCMD(linuxtv_t *pHandle, unsigned char *msg, unsigned int len)
{
	struct dvb_diseqc_master_cmd cmd;

	memcpy(cmd.msg, msg, len);
	cmd.msg_len = len;

	if (linuxtv_fe_ioctl_diseqc_send_master_cmd(pHandle, &cmd) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_DiSEqC_GetReply(linuxtv_t *pHandle, unsigned char *msg, unsigned int *len)
{
	struct dvb_diseqc_slave_reply reply;

	if (linuxtv_fe_ioctl_diseqc_recv_slave_reply(pHandle, &reply) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	memcpy(msg, reply.msg, reply.msg_len);
	*len = reply.msg_len;

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_DiSEqC_SendBurst(linuxtv_t *pHandle, int arg)
{
	if (linuxtv_fe_ioctl_diseqc_send_burst(pHandle, (fe_sec_mini_cmd_t)arg) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_SetTone(linuxtv_t *pHandle, int arg)
{
	if (linuxtv_fe_ioctl_set_tone(pHandle, (fe_sec_tone_mode_t)arg) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_SetVoltage(linuxtv_t *pHandle, int arg)
{
	if (linuxtv_fe_ioctl_set_voltage(pHandle, (fe_sec_voltage_t)arg) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_EnableHighLNBVoltage(linuxtv_t *pHandle, int arg)
{
	if (linuxtv_fe_ioctl_enable_high_lnb_voltage(pHandle, arg) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_GetStatus(linuxtv_t *pHandle, int *status)
{
	if (linuxtv_fe_ioctl_read_status(pHandle, (fe_status_t *)status) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_GetBER(linuxtv_t *pHandle, unsigned int *ber)
{
	unsigned int uiBER;

	if (linuxtv_fe_ioctl_read_ber(pHandle, &uiBER) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	*ber = uiBER;

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_GetSignalQuality(linuxtv_t *pHandle, unsigned int *uiQuality)
{
	unsigned short usQuality;

	if (linuxtv_fe_ioctl_read_snr(pHandle, &usQuality) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	*uiQuality = (unsigned int)(usQuality * 4);

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_GetSignalStrength(linuxtv_t *pHandle, unsigned int *str)
{
	unsigned short usSTR;

	if (linuxtv_fe_ioctl_read_signal_strength(pHandle, &usSTR) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	*str = (unsigned int)usSTR;

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_GetSNR(linuxtv_t *pHandle, unsigned int *snr)
{
	unsigned short usSNR;

	if (linuxtv_fe_ioctl_read_snr(pHandle, &usSNR) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	*snr = (unsigned int)usSNR;

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_GetUncorrectedBlocks(linuxtv_t *pHandle, unsigned int *ucblocks)
{
	if (linuxtv_fe_ioctl_read_uncorrected_blocks(pHandle, ucblocks) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_SetFrontend(linuxtv_t *pHandle)
{
	struct dtv_properties tvps;
	struct dtv_property *tvp;

	tvps.num = 5;
	tvps.props = malloc(sizeof(struct dtv_property) * tvps.num);

	tvp = tvps.props;

	linuxtv_fe_tvps_set(tvp++, DTV_CLEAR, 0);

	if (pHandle->iSystemType == LINUXTV_DVB_S2)
	{
		linuxtv_fe_tvps_set(tvp++, DTV_DELIVERY_SYSTEM, SYS_DVBS2);
		linuxtv_fe_tvps_set(tvp++, DTV_FREQUENCY,       pHandle->iFrequency);
		linuxtv_fe_tvps_set(tvp++, DTV_SYMBOL_RATE,     pHandle->iSymbolRate);
		linuxtv_fe_tvps_set(tvp++, DTV_TUNE,            0);
	}
	else if (pHandle->iSystemType == LINUXTV_DVB_T)
	{
		linuxtv_fe_tvps_set(tvp++, DTV_DELIVERY_SYSTEM, SYS_DVBT);
		linuxtv_fe_tvps_set(tvp++, DTV_FREQUENCY,       pHandle->iFrequency);
		linuxtv_fe_tvps_set(tvp++, DTV_BANDWIDTH_HZ,    pHandle->iBandwidth*1000);
		linuxtv_fe_tvps_set(tvp++, DTV_TUNE,            0);
	}
	else if (pHandle->iSystemType == LINUXTV_DVB_T2)
	{
		linuxtv_fe_tvps_set(tvp++, DTV_DELIVERY_SYSTEM, SYS_DVBT2);
		linuxtv_fe_tvps_set(tvp++, DTV_FREQUENCY,       pHandle->iFrequency);
		linuxtv_fe_tvps_set(tvp++, DTV_BANDWIDTH_HZ,    pHandle->iBandwidth*1000);
		linuxtv_fe_tvps_set(tvp++, DTV_TUNE,            0);
	}
	else if (pHandle->iSystemType == LINUXTV_ATSC_D)
	{
		linuxtv_fe_tvps_set(tvp++, DTV_DELIVERY_SYSTEM, SYS_ATSC);
		linuxtv_fe_tvps_set(tvp++, DTV_FREQUENCY,       pHandle->iFrequency);
		linuxtv_fe_tvps_set(tvp++, DTV_MODULATION,      pHandle->iModulation);
		linuxtv_fe_tvps_set(tvp++, DTV_TUNE,            0);
	}
	else if (pHandle->iSystemType == LINUXTV_ATSC_A)
	{
		linuxtv_fe_tvps_set(tvp++, DTV_DELIVERY_SYSTEM, SYS_ATSC);
		linuxtv_fe_tvps_set(tvp++, DTV_FREQUENCY,       pHandle->iFrequency);
		linuxtv_fe_tvps_set(tvp++, DTV_MODULATION,      pHandle->iModulation);
		linuxtv_fe_tvps_set(tvp++, DTV_TUNE,            0);
	}
	else
	{
		free(tvps.props);
		ALOGE("Unknown system type");
		return -1;
	}

	if (linuxtv_fe_ioctl_set_properties(pHandle, &tvps) != 0)
	{
		free(tvps.props);
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	free(tvps.props);

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_GetFrontend(linuxtv_t *pHandle)
{
	struct dvb_frontend_parameters Params;

	if (linuxtv_fe_ioctl_get_frontend(pHandle, &Params) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_SetTuneMode(linuxtv_t *pHandle, unsigned int arg)
{
	if (linuxtv_fe_ioctl_set_frontend_tune_mode(pHandle, arg) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_WaitLock(linuxtv_t *pHandle, int timeout)
{
	struct dvb_frontend_event event;

	if (linuxtv_fe_ioctl_get_event(pHandle, &event, timeout) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return (event.status & FE_HAS_LOCK) ? 0 : ((event.status & FE_TIMEDOUT) ? -1 : 1);
}

int LinuxTV_Frontend_DishNetwork_SendLegacyCMD(linuxtv_t *pHandle, unsigned int arg)
{
	if (linuxtv_fe_ioctl_dishnetwork_send_legacy_cmd(pHandle, arg) != 0)
	{
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_SetAntennaPower(linuxtv_t *pHandle, int arg)
{
	if (pHandle->iFrontend < 0)
		return 0;

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_BlindScan_Start(linuxtv_t *pHandle)
{
	struct dtv_properties tvps;
	struct dtv_property *tvp;

	if (pHandle->iSystemType != LINUXTV_DVB_S2)
		return -1;

	tvps.num = 5;
	tvps.props = malloc(sizeof(struct dtv_property) * tvps.num);

	tvp = tvps.props;

	pHandle->iBlindScanChannelIndex = 0;
	pHandle->iBlindScanChannelCount = 0;
	pHandle->iBlindScanProgress = 0;

	linuxtv_fe_tvps_set(tvp++, DTV_CLEAR,           0);
	linuxtv_fe_tvps_set(tvp++, DTV_DELIVERY_SYSTEM, SYS_DVBS2);
	linuxtv_fe_tvps_set(tvp++, DTV_FREQUENCY,       pHandle->stInfo.frequency_min);
	linuxtv_fe_tvps_set(tvp++, DTV_SYMBOL_RATE,     pHandle->stInfo.symbol_rate_min);
	linuxtv_fe_tvps_set(tvp++, DTV_TUNE,            1);

	if (linuxtv_fe_ioctl_set_properties(pHandle, &tvps) != 0)
	{
		free(tvps.props);
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	free(tvps.props);

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_BlindScan_Cancel(linuxtv_t *pHandle)
{
	struct dtv_properties tvps;
	struct dtv_property *tvp;

	if (pHandle->iSystemType != LINUXTV_DVB_S2)
		return -1;

	tvps.num = 1;
	tvps.props = malloc(sizeof(struct dtv_property) * tvps.num);

	tvp = tvps.props;

	linuxtv_fe_tvps_set(tvp++, DTV_CLEAR, 0);

	if (linuxtv_fe_ioctl_set_properties(pHandle, &tvps) != 0)
	{
		free(tvps.props);
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	free(tvps.props);

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_BlindScan_GetState(linuxtv_t *pHandle, int *state)
{
	struct dvb_frontend_event event;

	if (pHandle->iSystemType != LINUXTV_DVB_S2)
		return -1;

	*state = 0;

	while (linuxtv_fe_ioctl_get_event(pHandle, &event, 0) == 0)
	{
		if (event.status & FE_HAS_LOCK)
		{
			if (event.status & FE_HAS_SIGNAL)
			{
				ALOGE("[%s] Error!", __func__);
				*state = 2;
				return -1;
			}
			else
			{
				pHandle->iBlindScanProgress = event.parameters.frequency;
				pHandle->iBlindScanChannelCount = event.parameters.u.qpsk.symbol_rate;
				*state = 1;
			}
			break;
		}
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_BlindScan_GetInfo(linuxtv_t *pHandle, int *percent, int *index, int *freqMHz, int *symKHz)
{
	struct dvb_frontend_event event;

	if (pHandle->iSystemType != LINUXTV_DVB_S2)
		return -1;

	*percent = pHandle->iBlindScanProgress;
	*index = 0;
	*freqMHz = 0;
	*symKHz = 0;

	if (pHandle->iBlindScanChannelIndex < pHandle->iBlindScanChannelCount)
	{
		if (linuxtv_fe_ioctl_get_event(pHandle, &event, 1000) == 0)
		{
			if ((event.status & FE_HAS_LOCK) && (event.status & FE_HAS_SIGNAL))
			{
				pHandle->iBlindScanChannelIndex++;
				*index = pHandle->iBlindScanChannelIndex;
				*freqMHz = event.parameters.frequency / 1000;
				*symKHz = event.parameters.u.qpsk.symbol_rate / 1000;
			}
		}
		else
		{
			ALOGE("%s ERROR", __func__);
			return -1;
		}
	}

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_SetDataPLP(linuxtv_t *pHandle, int arg)
{
	struct dtv_properties tvps;
	struct dtv_property *tvp;

	if (pHandle->iSystemType != LINUXTV_DVB_T2)
		return -1;

	tvps.num = 1;
	tvps.props = malloc(sizeof(struct dtv_property) * tvps.num);

	tvp = tvps.props;

	linuxtv_fe_tvps_set(tvp++, DTV_DVBT2_PLP_ID_LEGACY, arg);

	if (linuxtv_fe_ioctl_set_properties(pHandle, &tvps) != 0)
	{
		free(tvps.props);
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	free(tvps.props);

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

int LinuxTV_Frontend_GetDataPLPs(linuxtv_t *pHandle, int *piPLPIds, int *piPLPNum)
{
	struct dtv_properties tvps;
	struct dtv_property *tvp;
	unsigned char pPLPIds[256];
	unsigned int  iNumPLPs = 256;
	int i;

	if (pHandle->iSystemType != LINUXTV_DVB_T2)
		return -1;

	tvps.num = 1;
	tvps.props = malloc(sizeof(struct dtv_property) * tvps.num);

	tvp = tvps.props;

	linuxtv_fe_tvps_set_ex(tvp++, DTV_DVBT2_PLP_ID_LEGACY, pPLPIds, iNumPLPs);

	if (linuxtv_fe_ioctl_get_properties(pHandle, &tvps) != 0)
	{
		free(tvps.props);
		ALOGE("[%s] Error!", __func__);
		return -1;
	}

	*piPLPNum = tvps.props[0].u.buffer.len;
	for (i = 0; i < *piPLPNum; i++)
		piPLPIds[i] = (unsigned int)pPLPIds[i];

	free(tvps.props);

	DEBUG_MSG("[%s]", __func__);
	return 0;
}

