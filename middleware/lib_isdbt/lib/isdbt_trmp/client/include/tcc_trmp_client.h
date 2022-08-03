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

#ifndef _TCC_TRMP_CLIENT_H_
#define _TCC_TRMP_CLIENT_H_

#include "tcc_trmp_type.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int tcc_trmp_client_init(void);
extern void tcc_trmp_client_deinit(void);
extern int tcc_trmp_client_reset(void);

extern void tcc_trmp_client_setDeviceKeyUpdateFunction(updateDeviceKey func);
extern int tcc_trmp_client_setEMM(
	unsigned short usNetworkID, unsigned char *pSectionData, int nSectionDataSize);
extern int tcc_trmp_client_setECM(
	unsigned short usNetworkID, unsigned char *pSectionData, int nSectionDataSize,
	unsigned char *pOddKey, unsigned char *pEvenKey);

extern void
tcc_trmp_client_getMulti2SystemKey(unsigned char *pSystemKey, unsigned char *pCBCDefaultValue);
extern int tcc_trmp_client_getIDInfoSize(void);
extern int tcc_trmp_client_getIDInfo(unsigned char *pIDInfo, int iIDInfoSize);

#ifdef __cplusplus
}
#endif

#endif
