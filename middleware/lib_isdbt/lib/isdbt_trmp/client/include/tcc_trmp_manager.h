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

#ifndef _TCC_TRMP_MANAGER_H_
#define _TCC_TRMP_MANAGER_H_

#include "tcc_trmp_type.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int tcc_trmp_manager_init(void);
extern void tcc_trmp_manager_deinit(void);
extern int tcc_trmp_manager_reset(void);

extern void tcc_trmp_manager_enableKeySwap(int nEnable);
extern void tcc_trmp_manager_setDeviceKeyUpdateFunction(updateDeviceKey func);

extern int tcc_trmp_manager_setEMM(
	unsigned short usNetworkID, unsigned char *pSectionData, int nSectionDataSize);
extern int tcc_trmp_manager_setECM(
	unsigned short usNetworkID, unsigned char *pSectionData, int nSectionDataSize,
	unsigned char *pOddKey, unsigned char *pEvenKey);

extern int
tcc_trmp_manager_getMulti2Info(unsigned char **ppSystemKey, unsigned char **ppCBCDefaultValue);
extern int tcc_trmp_manager_getIDInfo(unsigned char **ppInfo, int *piInfoSize);

#ifdef __cplusplus
}
#endif

#endif
