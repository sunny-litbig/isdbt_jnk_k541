/**

  OpenMAX Base Filter component. This component does not perform any multimedia
  processing. It derives from base component and contains two ports. It can be used 
  as base class for codec and filter components.

  Copyright (C) 2007-2008  STMicroelectronics
  Copyright (C) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
  Copyright (C) 2009-2010 Telechips Inc.

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA
  02110-1301  USA

*/

#ifndef __OMX_BASE_FILTER_H__
#define __OMX_BASE_FILTER_H__

#include <OMX_Types.h>
#include <OMX_Component.h>
#include <OMX_Core.h>
#include <pthread.h>
#include <stdlib.h>
#include <omx_base_component.h>
#include "TCCMemory.h"

/** OMX_BASE_FILTER_INPUTPORT_INDEX is the index of any input port for the derived components
 */
#define OMX_BASE_FILTER_INPUTPORT_INDEX 0

/** OMX_BASE_FILTER_OUTPUTPORT_INDEX is the index of any output port for the derived components
 */
#define OMX_BASE_FILTER_OUTPUTPORT_INDEX 1

#define OMX_BASE_FILTER_OUTPUTPORT_INDEX_TWO 2

#define OMX_BASE_FILTER_OUTPUTPORT_INDEX_THREE 3

/** OMX_BASE_FILTER_ALLPORT_INDEX as the standard specifies, the -1 value for port index is used to point to all the ports
 */
#define OMX_BASE_FILTER_ALLPORT_INDEX -1

/** Base Filter component private structure.
 */
DERIVEDCLASS(omx_base_filter_PrivateType, omx_base_component_PrivateType)
#define omx_base_filter_PrivateType_FIELDS omx_base_component_PrivateType_FIELDS \
  /** @param pPendingOutputBuffer pending Output Buffer pointer */ \
  OMX_BUFFERHEADERTYPE* pPendingOutputBuffer; \
  /** @param BufferMgmtCallback function pointer for algorithm callback */ \
  void (*BufferMgmtCallback)(OMX_COMPONENTTYPE* openmaxStandComp, OMX_BUFFERHEADERTYPE* inputbuffer, OMX_BUFFERHEADERTYPE* outputbuffer);
ENDCLASS(omx_base_filter_PrivateType)

/** 
 * @brief The base filter contructor for the OpenMAX ST components
 * 
 * @param openmaxStandComp the ST component to be initialized
 * @param cComponentName the OpenMAX string that describes the component
 *  
 * @return OMX_ErrorInsufficientResources if a memory allocation fails
 */
OMX_ERRORTYPE dxb_omx_base_filter_Constructor(OMX_COMPONENTTYPE *openmaxStandComp,OMX_STRING cComponentName);

/** @brief the base filter destructor for ST OpenMAX components
 * 
 * @param openmaxStandComp the ST OpenMAX component to be disposed
 */
OMX_ERRORTYPE dxb_omx_base_filter_Destructor(OMX_COMPONENTTYPE *openmaxStandComp);

/** This is the central function for component processing. It
 * is executed in a separate thread, is synchronized with 
 * semaphores at each port, those are released each time a new buffer
 * is available on the given port.
 */
void* dxb_omx_base_filter_BufferMgmtFunction(void* param);

/*
  * This is  the modification of omx_base_filter_BufferMgmtFunction.
  * It's used to get two different ES in dxb
  */
void* dxb_omx_twoport_filter_component_BufferMgmtFunction (void* param);

#endif
