/*
 * Copyright (C) 2012 Telechips, Inc.  All rights reserved.
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

#include <stdlib.h>

#include "OMX_Component.h"
#include "OMX_TCC_Index.h"

#include "isdbt_tuner.h"

//ISDBT
OMX_ERRORTYPE omx_tcc353x_CSPI_STS_tuner_component_Init(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE omx_tcc353x_CSPI_STS_tuner_component_Deinit(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE omx_tcc353x_I2C_STS_tuner_component_Init(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE omx_tcc353x_I2C_STS_tuner_component_Deinit(OMX_COMPONENTTYPE *openmaxStandComp);

static int ISDBT_BBType = -1;

static int isdbt_tuner_set(OMX_COMPONENTTYPE *pOpenmaxStandComp, int iFreq, int iBW)
{
	OMX_ERRORTYPE eError;
	int iARGS[4];

	iARGS[0] = iFreq;
	iARGS[1] = iBW;
	iARGS[2] = 1;
	iARGS[3] = 0;

	eError = OMX_SetParameter(pOpenmaxStandComp, (OMX_INDEXTYPE)OMX_IndexVendorParamTunerFrequencySet, iARGS);
	if (eError == OMX_ErrorNone)
	{
		return 0;
	}

	return -1;
}

OMX_COMPONENTTYPE* isdbt_tuner_open(int iFreq, int iBW)
{
	OMX_ERRORTYPE eError;
	int iARGS[3];
	OMX_COMPONENTTYPE *pOpenmaxStandComp;

	iARGS[0] = 0;
	iARGS[1] = 1;
	iARGS[2] = 2;

	pOpenmaxStandComp = (OMX_COMPONENTTYPE *)calloc(1,sizeof(OMX_COMPONENTTYPE));
	if (pOpenmaxStandComp == NULL)
	{
		return NULL;
	}

	ISDBT_BBType = 0;
	eError = omx_tcc353x_CSPI_STS_tuner_component_Init(pOpenmaxStandComp);
	if (eError == OMX_ErrorNone)
	{
		eError = OMX_SetParameter(pOpenmaxStandComp, (OMX_INDEXTYPE)OMX_IndexVendorParamTunerOpen, iARGS);
		if (eError == OMX_ErrorNone)
		{
			if (isdbt_tuner_set(pOpenmaxStandComp, iFreq, iBW) == 0)
			{
				return pOpenmaxStandComp;
			}
		}
		OMX_SetParameter(pOpenmaxStandComp, (OMX_INDEXTYPE)OMX_IndexVendorParamTunerClose, NULL);

	}
	omx_tcc353x_CSPI_STS_tuner_component_Deinit(pOpenmaxStandComp);

	ISDBT_BBType = 1;
	eError = omx_tcc353x_I2C_STS_tuner_component_Init(pOpenmaxStandComp);
	if (eError == OMX_ErrorNone)
	{	
		eError = OMX_SetParameter(pOpenmaxStandComp, (OMX_INDEXTYPE)OMX_IndexVendorParamTunerOpen, iARGS);
		if (eError == OMX_ErrorNone)
		{
			if (isdbt_tuner_set(pOpenmaxStandComp, iFreq, iBW) == 0)
			{
				return pOpenmaxStandComp;
			}
		}
		OMX_SetParameter(pOpenmaxStandComp, (OMX_INDEXTYPE)OMX_IndexVendorParamTunerClose, NULL);
	}
	omx_tcc353x_I2C_STS_tuner_component_Deinit(pOpenmaxStandComp);

	free(pOpenmaxStandComp);
	ISDBT_BBType = -1;

	return NULL;
}

void isdbt_tuner_close(OMX_COMPONENTTYPE *pOpenmaxStandComp)
{
	if (ISDBT_BBType < 0 || pOpenmaxStandComp == NULL)
	{
		return;
	}

	OMX_SetParameter(pOpenmaxStandComp, (OMX_INDEXTYPE)OMX_IndexVendorParamTunerClose, NULL);

	if (ISDBT_BBType == 0)
	{
		omx_tcc353x_CSPI_STS_tuner_component_Deinit(pOpenmaxStandComp);
	}
	else if (ISDBT_BBType == 1)
	{
		omx_tcc353x_I2C_STS_tuner_component_Deinit(pOpenmaxStandComp);
	}

	free(pOpenmaxStandComp);
	ISDBT_BBType = -1;
}
