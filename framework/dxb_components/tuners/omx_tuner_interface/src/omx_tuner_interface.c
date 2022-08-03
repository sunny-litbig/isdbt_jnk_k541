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

#define LOG_NDEBUG 1
#define LOG_TAG "TUNER_INTERFACE"
#include <utils/Log.h>

#include <OMX_Core.h>
#include <OMX_Component.h>
#include <OMX_Types.h>
#include <OMX_Audio.h>
#include "OMX_Other.h"

#include <user_debug_levels.h>

#include "omx_tuner_interface.h"

#ifndef	NULL
#define	NULL	(0)
#endif

/*
  * 		ISDB-T
  *
  */
OMX_ERRORTYPE (*tcc_omx_isdbt_tuner_default (void))(OMX_COMPONENTTYPE *openaxStandComp)
{
	return	dxb_omx_tcc353x_CSPI_STS_tuner_component_Init;
}
OMX_ERRORTYPE  (*(pomx_isdbt_tuner_component_init[]))(OMX_COMPONENTTYPE *openmaxStandComp) =
{
	NULL,												// 0 - none
	NULL,		// 1 - TCC351X CSPI+STS
	NULL,					// 2 -Dib10096
	NULL,			// 3 - TCC351X I2C+STS
	NULL,					// 4 - NMI326
	NULL,		// 5 - TCC351X I2C+SPIMS
	NULL,			// 6 - MTV818
	NULL,	// 7 - Toshiba TC90517
	dxb_omx_tcc353x_CSPI_STS_tuner_component_Init,  // 8 - TCC353X CSPI+STS
	dxb_omx_tcc353x_I2C_STS_tuner_component_Init,   // 9 - TCC353X CSPI+STS
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
};
 int	tcc_omx_isdbt_tuner_count (void)
{
	return 	(sizeof(pomx_isdbt_tuner_component_init)/sizeof(pomx_isdbt_tuner_component_init[0]));
}
