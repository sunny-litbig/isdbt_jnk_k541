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

#include <OMX_Types.h>
#include <OMX_Component.h>
#include <OMX_Core.h>
#include <omx_base_source.h>
#include <omx_base_port.h> 

#include "LinuxTV_Frontend.h"

DERIVEDCLASS(omx_linuxtv_tuner_component_PrivateType, omx_base_source_PrivateType)
#define omx_linuxtv_tuner_component_PrivateType_FIELDS omx_base_source_PrivateType_FIELDS \
	linuxtv_t handles[2];
ENDCLASS(omx_linuxtv_tuner_component_PrivateType)

OMX_ERRORTYPE dxb_omx_linuxtv_tuner_component_Init(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE dxb_omx_linuxtv_tuner_component_Deinit(OMX_COMPONENTTYPE *openmaxStandComp);



