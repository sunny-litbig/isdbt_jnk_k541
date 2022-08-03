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

#define LOG_TAG	"OMX_LINUXTV_TUNER"
#include <utils/Log.h>

#include <OMX_TCC_Index.h>

#include <omxcore.h>
#include <omx_linuxtv_tuner_component.h>

#include <tcc_dxb_control.h>
#include <sys/ioctl.h>
#include <fcntl.h>

static OMX_ERRORTYPE dxb_omx_linuxtv_tuner_component_wait_lock(linuxtv_t *pHandle)
{
	unsigned int uiCounter = 20; // 100ms * 20 = 2s
	int status;
	do
	{
		status = LinuxTV_Frontend_WaitLock(pHandle, 100);
		if (status == 0)
			return OMX_ErrorNone;

		if (status == -1)
			break;
	} while(--uiCounter);

	return OMX_ErrorTimeout;
}

static int dxb_omx_linuxtv_tuner_component_dxb_power_on(linuxtv_t *pHandle)
{
	pHandle->iTccDxbCtrl = open(DXB_CTRL_DEV_FILE, O_RDWR | O_NDELAY);
	if (pHandle->iTccDxbCtrl < 0)
	{
		ALOGD ("%s CANNOT open %s :: %d\n", __func__, DXB_CTRL_DEV_FILE, pHandle->iTccDxbCtrl);
		return -1;
	}

	// Set the board type
	ioctl(pHandle->iTccDxbCtrl, IOCTL_DXB_CTRL_SET_BOARD, &pHandle->iBoardType);

	// Set the mode for power control of antenna
	ioctl(pHandle->iTccDxbCtrl, IOCTL_DXB_CTRL_SET_CTRLMODE, &pHandle->iAntennaCtrl);

	// Power On
	ioctl(pHandle->iTccDxbCtrl, IOCTL_DXB_CTRL_ON, &pHandle->iDeviceIdx);

	return 0;
}

static int dxb_omx_linuxtv_tuner_component_dxb_power_off(linuxtv_t *pHandle)
{
	// Power Off
	ioctl(pHandle->iTccDxbCtrl, IOCTL_DXB_CTRL_OFF, &pHandle->iDeviceIdx);

	close(pHandle->iTccDxbCtrl);

	return 0;
}

static void dxb_omx_linuxtv_tuner_component_BufferMgmtCallback(OMX_COMPONENTTYPE *openmaxStandComp, OMX_BUFFERHEADERTYPE* outputbuffer)
{
	outputbuffer->nFilledLen = 0;
}

static OMX_ERRORTYPE dxb_omx_linuxtv_tuner_component_DoStateSet(OMX_COMPONENTTYPE *openmaxStandComp, OMX_U32 destinationState)
{
	return OMX_ErrorNone;
}

static OMX_ERRORTYPE dxb_omx_linuxtv_tuner_component_MessageHandler(OMX_COMPONENTTYPE* openmaxStandComp, internalRequestMessageType *message)
{
	omx_linuxtv_tuner_component_PrivateType* omx_linuxtv_tuner_component_Private = openmaxStandComp->pComponentPrivate;
	OMX_STATETYPE eCurrentState = omx_linuxtv_tuner_component_Private->state;
	OMX_ERRORTYPE omxErr;

	if (message->messageType == OMX_CommandStateSet)
	{
		if ((eCurrentState == OMX_StateExecuting) && (message->messageParam == OMX_StateIdle))
		{
			DEBUG(DEB_LEV_ERR, "[DxBTUNER] OMX_StateExecuting -> OMX_StateIdle\n");
		}
	}

	omxErr = dxb_omx_base_component_MessageHandler(openmaxStandComp, message);

	if (message->messageType == OMX_CommandStateSet)
	{
		if ((eCurrentState == OMX_StateIdle) && (message->messageParam == OMX_StateExecuting))
		{
			DEBUG(DEB_LEV_ERR, "[DxBTUNER] OMX_StateIdle -> OMX_StateExecuting\n");
		}
	}

	return omxErr;
}

static OMX_ERRORTYPE dxb_omx_linuxtv_tuner_component_SetParameter(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_INDEXTYPE nParamIndex, OMX_IN OMX_PTR ComponentParameterStructure)
{
	OMX_ERRORTYPE omxErr = OMX_ErrorNone;
	OMX_COMPONENTTYPE *openmaxStandComp = (OMX_COMPONENTTYPE*)hComponent;
	omx_linuxtv_tuner_component_PrivateType* omx_linuxtv_tuner_component_Private = openmaxStandComp->pComponentPrivate;
	OMX_S32* piArg;
	linuxtv_t *pHandle;

	DEBUG(DEFAULT_MESSAGES, "In %s(nParamIndex = %d)\n", __func__, nParamIndex);

	switch((TC_OMX_INDEXVENDORTYPE)nParamIndex) 
	{		
		case OMX_IndexVendorParamTunerDeviceSet:
			piArg = (OMX_S32*)ComponentParameterStructure;
			pHandle = &omx_linuxtv_tuner_component_Private->handles[0];
			pHandle->iIndex = *piArg;
			break;

		case OMX_IndexVendorParamTunerChannelScan:
			break;

		case OMX_IndexVendorParamTunerGetChannelIndexByFrequency:
			break;

		case OMX_IndexVendorParamTunerModulationSet:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				pHandle->iConfigChanged = 1;
				switch((OMX_S32)piArg)
				{
					case 0:   pHandle->iModulation = QAM_AUTO; break;
					case 8:   pHandle->iModulation = VSB_8;    break;
					case 16:  pHandle->iModulation = VSB_16;   break;
					case 32:  pHandle->iModulation = QAM_32;   break;
					case 64:  pHandle->iModulation = QAM_64;   break;
					case 128: pHandle->iModulation = QAM_128;  break;
					case 256: pHandle->iModulation = QAM_256;  break;
					default:  pHandle->iModulation =  VSB_8;   break;
				}
			}
			break;

		case OMX_IndexVendorParamTunerChannelSearch:
		case OMX_IndexVendorParamTunerAnalogChannelSearch:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				if (pHandle->iSystemType == LINUXTV_ATSC_A && ((int)nParamIndex ==(int) OMX_IndexVendorParamTunerChannelSearch))
					pHandle->iSystemType = LINUXTV_ATSC_D;
				if (pHandle->iSystemType == LINUXTV_ATSC_D && ((int)nParamIndex == (int)OMX_IndexVendorParamTunerAnalogChannelSearch))
					pHandle->iSystemType = LINUXTV_ATSC_A;

				pHandle->iFrequency = (int) piArg;

				if (LinuxTV_Frontend_SetFrontend(pHandle) < 0)
				{
					DEBUG(DEB_LEV_ERR, "Fail to search Frequency(SystemType=%d, Freq=%d)\n", pHandle->iSystemType, pHandle->iFrequency);
					omxErr = OMX_ErrorBadParameter;
				}
				else
				{
					pHandle->iConfigChanged = 0;
					if (dxb_omx_linuxtv_tuner_component_wait_lock(pHandle) < 0)
					{
						DEBUG(DEB_LEV_ERR, "Fail to lock(SystemType=%d, Freq=%dkhz\n", pHandle->iSystemType, pHandle->iFrequency);
						omxErr = OMX_ErrorBadParameter;
					}
				}
			}
			break;

		case OMX_IndexVendorParamTunerChannelSet:
		case OMX_IndexVendorParamTunerAnalogChannelSet:
		case OMX_IndexVendorParamTunerFrequencySet:
			{
				int uiLock;

				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				piArg = (OMX_S32*)(*piArg);

				if (pHandle->iSystemType == LINUXTV_ATSC_A && ((int)nParamIndex == (int)OMX_IndexVendorParamTunerChannelSet)) {
					pHandle->iSystemType = LINUXTV_ATSC_D;
					pHandle->iConfigChanged = 1;
				}
				if (pHandle->iSystemType == LINUXTV_ATSC_D && ((int)nParamIndex == (int)OMX_IndexVendorParamTunerAnalogChannelSet)) {
					pHandle->iSystemType = LINUXTV_ATSC_A;
					pHandle->iConfigChanged = 1;
				}

				if ((int)nParamIndex != (int)OMX_IndexVendorParamTunerFrequencySet)
				{
					//piArg[0] : Freq, piArg[1]: LockOn
					uiLock = piArg[1];
				}
				else
				{
					//piArg[0] : Freq, piArg[1]: BW, piArg[2]: LockOn, piArg[3] : option
					uiLock = piArg[2];

					if (pHandle->iSystemType == LINUXTV_ATSC_A || pHandle->iSystemType == LINUXTV_ATSC_D)
					{
						if (pHandle->iModulation != piArg[1])
						{
							pHandle->iModulation = piArg[1];
							pHandle->iConfigChanged = 1;
						}
					}
					else if (pHandle->iSystemType == LINUXTV_DVB_S2 || pHandle->iSystemType == LINUXTV_DVB_S)
					{
						if (pHandle->iSymbolRate != piArg[1])
						{
							pHandle->iSymbolRate = piArg[1];
							pHandle->iConfigChanged = 1;
						}
					}
					else if (pHandle->iSystemType == LINUXTV_DVB_T2 || pHandle->iSystemType == LINUXTV_DVB_T)
					{
						if ((int)piArg[3] == 0 && pHandle->iSystemType == LINUXTV_DVB_T2)
						{
							pHandle->iSystemType = LINUXTV_DVB_T;
							pHandle->iConfigChanged = 1;
						}
						if ((int)piArg[3] != 0 && pHandle->iSystemType == LINUXTV_DVB_T)
						{
							pHandle->iSystemType = LINUXTV_DVB_T2;
							pHandle->iConfigChanged = 1;
						}
						if (pHandle->iBandwidth != piArg[1])
						{
							pHandle->iBandwidth = piArg[1];
							pHandle->iConfigChanged = 1;
						}
					}
				}
				if (pHandle->iFrequency != piArg[0]) {
					pHandle->iFrequency = piArg[0];
					pHandle->iConfigChanged = 1;
				}
				if (pHandle->iConfigChanged == 1 || uiLock == 1)
				{
					if (LinuxTV_Frontend_SetFrontend(pHandle) < 0)
					{
						DEBUG(DEB_LEV_ERR, "Fail to set Frequency(SystemType=%d, Freq=%dkhz\n", pHandle->iSystemType, pHandle->iFrequency);
						omxErr = OMX_ErrorBadParameter;
					}
					else
					{
						pHandle->iConfigChanged = 0;
						if (uiLock == 1)
						{
							if (dxb_omx_linuxtv_tuner_component_wait_lock(pHandle) < 0)
							{
								DEBUG(DEB_LEV_ERR, "Fail to lock(SystemType=%d, Freq=%dkhz\n", pHandle->iSystemType, pHandle->iFrequency);
								omxErr = OMX_ErrorBadParameter;
							}
						}
					}
				}
			}
			break;

		case OMX_IndexVendorParamTunerCountryCodeSet:
			break;

		case OMX_IndexVendorParamTunerOpen:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				piArg = (OMX_S32*)(*piArg);

				//piArg[0] : countrycode, piArg[1]: standard, piArg[2]: baseband
				pHandle->iBoardType = BOARD_DVBT_DIB7070; // default
				pHandle->iDeviceIdx = 0;
				pHandle->iAntennaCtrl = 0;

				if (piArg[1] == (LINUXTV_DVB >> 8))
				{
					if (piArg[2] == 1)
					{
						pHandle->iBoardType = BOARD_DVBS2_AVL6211_YJ;
						pHandle->iSystemType = LINUXTV_DVB_S2;
					}
					else if (piArg[2] == 4)
					{
						pHandle->iBoardType = BOARD_DVBT_MXL101SF_YJ;
						pHandle->iSystemType = LINUXTV_DVB_T;
					}
					else if (piArg[2] == 5)
					{
						pHandle->iBoardType = BOARD_DVBT2_MN88472_YJ;
						pHandle->iSystemType = LINUXTV_DVB_T2;
					}
				}
				else if (piArg[1] == (LINUXTV_ATSC >> 8))
				{
					pHandle->iSystemType = LINUXTV_ATSC_D;
				}
				else
				{
					DEBUG(DEB_LEV_ERR, "Fail to open tuner\n");
					omxErr = OMX_ErrorBadParameter;
					break;
				}
				pHandle->iConfigChanged = 1;
				dxb_omx_linuxtv_tuner_component_dxb_power_on(pHandle);
				LinuxTV_Frontend_Open(pHandle);
				//LinuxTV_Frontend_SetTuneMode(pHandle, FE_TUNE_MODE_ONESHOT);
			}
			break;

		case OMX_IndexVendorParamTunerClose:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				LinuxTV_Frontend_Close(pHandle);
				dxb_omx_linuxtv_tuner_component_dxb_power_off(pHandle);
			}
			break;

		case OMX_IndexVendorParamSetResync:
			break;

		case OMX_IndexVendorParamRegisterPID:
			break;

		case OMX_IndexVendorParamUnRegisterPID:
			break;

		case OMX_IndexVendorParamTunerSetNumberOfBB:
			break;

		case OMX_IndexVendorParamTunerSetDataPLP:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				if (LinuxTV_Frontend_SetDataPLP(pHandle, (int)piArg) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;

		case OMX_IndexVendorParamSetAntennaPower:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				if (LinuxTV_Frontend_SetAntennaPower(pHandle, (int)piArg) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;

		case OMX_IndexVendorParamSetVoltage:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				if (LinuxTV_Frontend_SetVoltage(pHandle, (int)piArg) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;

		case OMX_IndexVendorParamSetTone:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				if (LinuxTV_Frontend_SetTone(pHandle, (int)piArg) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;

		case OMX_IndexVendorParamDiSEqCSendBurst:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				if (LinuxTV_Frontend_DiSEqC_SendBurst(pHandle, (int)piArg) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;

		case OMX_IndexVendorParamDiSEqCSendCMD:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				piArg = (OMX_S32*)(*piArg);

				// piArg[0]: message, piArg[1]: length of message
				if (LinuxTV_Frontend_DiSEqC_SendCMD(pHandle, (unsigned char *)piArg[0], (unsigned int)piArg[1]) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;

		case OMX_IndexVendorParamBlindScanReset:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				if (LinuxTV_Frontend_BlindScan_Start(pHandle) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;

		case OMX_IndexVendorParamBlindScanStart:
			break;

		case OMX_IndexVendorParamBlindScanCancel:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				if (LinuxTV_Frontend_BlindScan_Cancel(pHandle) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;

		default: /*Call the base component function*/
			return dxb_omx_base_component_SetParameter(hComponent, nParamIndex, ComponentParameterStructure);
	}

	return omxErr;
}

static OMX_ERRORTYPE dxb_omx_linuxtv_tuner_component_GetParameter(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_INDEXTYPE nParamIndex, OMX_INOUT OMX_PTR ComponentParameterStructure)
{
	OMX_ERRORTYPE omxErr = OMX_ErrorNone;
	OMX_COMPONENTTYPE *openmaxStandComp = (OMX_COMPONENTTYPE*)hComponent;
	omx_linuxtv_tuner_component_PrivateType* omx_linuxtv_tuner_component_Private = openmaxStandComp->pComponentPrivate;
	OMX_S32* piArg;
	linuxtv_t *pHandle;

	DEBUG(DEFAULT_MESSAGES, "In %s(nParamIndex = %d)\n", __func__, nParamIndex);

	if (ComponentParameterStructure == NULL) 
	{
		return OMX_ErrorBadParameter;
	}
	DEBUG(DEB_LEV_SIMPLE_SEQ, "   Getting parameter %i\n", nParamIndex);

	switch((TC_OMX_INDEXVENDORTYPE)nParamIndex) 
	{		
		case OMX_IndexVendorParamTunerChannelSet:
			break;

		case OMX_IndexVendorParamTunerChannelSearchStart:
			break;

		case OMX_IndexVendorParamTunerChannelSearchStop:
			break;

		case OMX_IndexVendorParamSetGetPacketDataEntry:
			break;

		case OMX_IndexVendorParamGetSignalStrength:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				piArg = (OMX_S32*)(*piArg);

				LinuxTV_Frontend_GetSignalStrength(pHandle, (unsigned int*)&piArg[0]); // signal strength (0 ~ 100)
				LinuxTV_Frontend_GetSignalQuality(pHandle, (unsigned int*)&piArg[1]);  // signal quality (0 ~ 100)
				//LinuxTV_Frontend_GetSNR(pHandle, (OMX_U32*)&piArg[2]);		  // snr
				//LinuxTV_Frontend_GetBER(pHandle, (OMX_U32*)&piArg[3]);		  // ber
				//if (piArg[4]) sprintf((char *)piArg[4], "STR: %d, QUA: %d, SNR: %d, BER: %d\n", piArg[0], piArg[1], piArg[2], piArg[3]);
			}
			break;

		case OMX_IndexVendorParamBlindScanState:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				piArg = (OMX_S32*)(*piArg);

				if (LinuxTV_Frontend_BlindScan_GetState(pHandle, (int*)piArg) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;

		case OMX_IndexVendorParamBlindScanInfo:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				piArg = (OMX_S32*)(*piArg);

				if (LinuxTV_Frontend_BlindScan_GetInfo(pHandle, (int*)&piArg[0], (int*)&piArg[1], (int*)&piArg[2], (int*)&piArg[3]) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;			

		case OMX_IndexVendorParamTunerGetDataPLPs:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				piArg = (OMX_S32*)(*piArg);

				if (LinuxTV_Frontend_GetDataPLPs(pHandle, (int*)piArg[0], (int*)piArg[1]) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;			

		case OMX_IndexVendorParamTunerInformation:
			{
				piArg = (OMX_S32*)ComponentParameterStructure;
				pHandle = &omx_linuxtv_tuner_component_Private->handles[*(piArg++)];

				piArg = (OMX_S32*)(*piArg);

				if( LinuxTV_Frontend_GetInfo(pHandle, (int*)piArg[0], (unsigned int*)piArg[1]) != 0)
					omxErr = OMX_ErrorBadParameter;
			}
			break;

		case OMX_IndexVendorParamTunerGetEWSFlag:
			break;

		default: /*Call the base component function*/
			return dxb_omx_base_component_GetParameter(hComponent, nParamIndex, ComponentParameterStructure);
	}

	return omxErr;
}

static OMX_ERRORTYPE dxb_omx_linuxtv_tuner_component_Destructor(OMX_COMPONENTTYPE *openmaxStandComp)
{
	omx_linuxtv_tuner_component_PrivateType* omx_linuxtv_tuner_component_Private = openmaxStandComp->pComponentPrivate;
	OMX_U32 i;
	OMX_ERRORTYPE omxErr;

	DEBUG(DEFAULT_MESSAGES, "In %s\n", __func__);

	if (omx_linuxtv_tuner_component_Private->ports)
	{
		for (i = 0; i < omx_linuxtv_tuner_component_Private->sPortTypesParam[OMX_PortDomainOther].nPorts; i++)
		{
			if(omx_linuxtv_tuner_component_Private->ports[i])
			{
				omx_linuxtv_tuner_component_Private->ports[i]->PortDestructor(omx_linuxtv_tuner_component_Private->ports[i]);
			}
		}
		TCC_free(omx_linuxtv_tuner_component_Private->ports);
		omx_linuxtv_tuner_component_Private->ports = NULL;
	}

	return dxb_omx_base_source_Destructor(openmaxStandComp);
}

static OMX_ERRORTYPE dxb_omx_linuxtv_tuner_component_Constructor(OMX_COMPONENTTYPE *openmaxStandComp, OMX_STRING cComponentName)
{
	omx_linuxtv_tuner_component_PrivateType* omx_linuxtv_tuner_component_Private;
	OMX_U32 i;

	DEBUG(DEFAULT_MESSAGES, "In %s\n", __func__);

	if (!openmaxStandComp->pComponentPrivate)
	{
		openmaxStandComp->pComponentPrivate = TCC_calloc(1, sizeof(omx_linuxtv_tuner_component_PrivateType));
		if (!openmaxStandComp->pComponentPrivate)
		{
			return OMX_ErrorInsufficientResources;
		}
	}

	if (dxb_omx_base_source_Constructor(openmaxStandComp, cComponentName) != OMX_ErrorNone)
	{
		return OMX_ErrorInsufficientResources;
	}

	omx_linuxtv_tuner_component_Private = (omx_linuxtv_tuner_component_PrivateType*)openmaxStandComp->pComponentPrivate;

	omx_linuxtv_tuner_component_Private->sPortTypesParam[OMX_PortDomainOther].nStartPortNumber = 0;
	omx_linuxtv_tuner_component_Private->sPortTypesParam[OMX_PortDomainOther].nPorts = 1;

	if (omx_linuxtv_tuner_component_Private->sPortTypesParam[OMX_PortDomainOther].nPorts && !omx_linuxtv_tuner_component_Private->ports)
	{
		omx_linuxtv_tuner_component_Private->ports = TCC_calloc(omx_linuxtv_tuner_component_Private->sPortTypesParam[OMX_PortDomainOther].nPorts, sizeof(omx_base_PortType *));
		if (!omx_linuxtv_tuner_component_Private->ports)
		{
			return OMX_ErrorInsufficientResources;
		}
		for (i = 0; i < omx_linuxtv_tuner_component_Private->sPortTypesParam[OMX_PortDomainOther].nPorts; i++)
		{
			omx_linuxtv_tuner_component_Private->ports[i] = TCC_calloc(1, sizeof(omx_base_PortType));
			if (!omx_linuxtv_tuner_component_Private->ports[i])
			{
				return OMX_ErrorInsufficientResources;
			}
		}
	}
	dxb_base_port_Constructor(openmaxStandComp, &omx_linuxtv_tuner_component_Private->ports[0], 0, OMX_FALSE);

	omx_linuxtv_tuner_component_Private->BufferMgmtCallback = dxb_omx_linuxtv_tuner_component_BufferMgmtCallback;
	omx_linuxtv_tuner_component_Private->destructor = dxb_omx_linuxtv_tuner_component_Destructor;
	omx_linuxtv_tuner_component_Private->messageHandler = dxb_omx_linuxtv_tuner_component_MessageHandler;
	omx_linuxtv_tuner_component_Private->DoStateSet = dxb_omx_linuxtv_tuner_component_DoStateSet;

	openmaxStandComp->SetParameter  = dxb_omx_linuxtv_tuner_component_SetParameter;
	openmaxStandComp->GetParameter  = dxb_omx_linuxtv_tuner_component_GetParameter;

	omx_linuxtv_tuner_component_Private->handles[0].iFrontend = -1;
	omx_linuxtv_tuner_component_Private->handles[0].iIndex = 0;
	omx_linuxtv_tuner_component_Private->handles[1].iFrontend = -1;
	omx_linuxtv_tuner_component_Private->handles[1].iIndex = 1;

	return OMX_ErrorNone;
}

OMX_ERRORTYPE dxb_omx_linuxtv_tuner_component_Init(OMX_COMPONENTTYPE *openmaxStandComp)
{
	return dxb_omx_linuxtv_tuner_component_Constructor(openmaxStandComp, "OMX.tcc.broadcast.dxb_tuner");
}

OMX_ERRORTYPE dxb_omx_linuxtv_tuner_component_Deinit(OMX_COMPONENTTYPE *openmaxStandComp)
{
	return dxb_omx_linuxtv_tuner_component_Destructor(openmaxStandComp);
}
