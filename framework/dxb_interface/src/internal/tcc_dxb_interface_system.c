/****************************************************************************

Copyright (C) 2013 Telechips Inc.


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions 
andlimitations under the License.

****************************************************************************/

/****************************************************************************

  Revision History

 ****************************************************************************

 ****************************************************************************/
#define LOG_NDEBUG 0
#define LOG_TAG	"DXB_INTERFACE_SYSTEM"
#include <OMX_Core.h>
#include <OMX_Component.h>
#include <OMX_Types.h>

#include <utils/Log.h>
#include <user_debug_levels.h>
#include <omx_base_component.h>

#include "tcc_dxb_interface_type.h"
#include "tcc_dxb_interface_omxil.h"
#include "tcc_dxb_interface_system.h"
 
DxB_ERR_CODE TCC_DxB_SYSTEM_Init(void)
{
	return DxB_ERR_OK;
}

DxB_ERR_CODE TCC_DxB_SYSTEM_Exit(void)
{

	return DxB_ERR_OK;
}
