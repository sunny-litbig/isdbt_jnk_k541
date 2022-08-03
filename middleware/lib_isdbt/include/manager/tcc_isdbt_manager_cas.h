/*
 * Copyright (C) 2013 Telechips, Inc.
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

#ifndef	_TCC_ISDBT_MANAGER_CAS_H_
#define	_TCC_ISDBT_MANAGER_CAS_H_

#include "tcc_trmp_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ISDBT_CAS_SUCCESS					0
#define ISDBT_CAS_ERROR						(-1)
#define ISDBT_CAS_ERROR_UNSUPPORTED			(-2)
#define ISDBT_CAS_ERROR_INVALID_PARAM		(-3)
#define ISDBT_CAS_ERROR_QUEUE_FULL			(-4)
#define ISDBT_CAS_ERROR_CIPHER				(-5)
#define ISDBT_CAS_ERROR_SMART_CARD			(-6)
#define ISDBT_CAS_ERROR_TRMP_MODULE			(-7)

#define ISDBT_CAS_CIPHER_TYPE_MASK 			(7<<1)		/* 1=built-in cipher block, 2=s/w, 4=tcc353x, 0=not defined */
#define ISDBT_CAS_CIPHER_HW					(1<<1)
#define ISDBT_CAS_CIPHER_SW					(2<<1)
#define ISDBT_CAS_CIPHER_TCC353X			(4<<1)
#define ISDBT_CAS_CIPHER_HWDEMUX			(8<<1)

#define ISDBT_CAS_SECTION_HEADER_LENGTH		8
#define ISDBT_CAS_SECTION_CRC_LENGTH		4
#define	ISDBT_CAS_SECTION_QUEUESIZE			1000

#define ISDBT_CA_SYSTEM_ID_CA5				0x0005
#define ISDBT_CA_SYSTEM_ID_RMP						0x000E

#define ISDBT_TRANSMISSION_TYPE_RMP					0x7

#define ISDBT_CAS_EMM_TIME_LIMITE			30

extern int tcc_manager_cas_pushSectionData(void *data);
extern int tcc_manager_cas_init(void);
extern int tcc_manager_cas_deinit(void);
extern int tcc_manager_cas_reset(void);
extern int tcc_manager_cas_resetInfo(void);
extern int tcc_manager_cas_setDeviceKeyUpdateFunction(updateDeviceKey func);
extern int tcc_manager_cas_getInfo(void **ppInfo, int *pInfoSize);
extern int tcc_manager_cas_getCipherType (void);

#ifdef __cplusplus
}
#endif

#endif
