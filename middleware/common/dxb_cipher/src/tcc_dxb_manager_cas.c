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

#define LOG_NDEBUG 0
#include <utils/Log.h>

#include "tcc_dxb_manager_cas.h"
#include "tcc_dxb_manager_dsc.h"


/*****************************************************************************************
define
*****************************************************************************************/
#define DSC_PRINTF		//printf


typedef enum TCCDEMUX_DESC_AlgorithmVariant
{
	TCCDEMUX_DESC_AlgorithmVariant_eEcb,
	TCCDEMUX_DESC_AlgorithmVariant_eCbc,
	TCCDEMUX_DESC_AlgorithmVariant_eCounter,
	/* Add new algorithm variant definition before this line */
	TCCDEMUX_DESC_AlgorithmVariant_eMax
}   TCCDEMUX_DESC_AlgorithmVariant;

typedef struct TCCDEMUX_DESC_Multi2_Cbc
{
	unsigned int cbc_l;
	unsigned int cbc_r;	
} TCCDEMUX_DESC_Multi2_Cbc;

typedef struct TCCDEMUX_DESC_EncryptionSettings
{
	TCCDEMUX_DESC_AlgorithmVariant	algorithmVar;
	int				bIsUsed;
	unsigned short	multi2KeySelect;
	unsigned char	multi2Rounds;
	unsigned char	multi2SysKey[32];
} TCCDEMUX_DESC_EncryptionSettings;


static unsigned int BCAS_MULTI2_SW_HANDLE =  0;

/*****************************************************************************************
Interface API
*****************************************************************************************/

/**************************************************************************
 FUNCTION NAME : TCC_DxB_CAS_Open
 
 DESCRIPTION : cipher_block �� open �ϰ� mode setting�� �Ѵ�.  
 
 INPUT	 :  encryptionSetting - Parameter
 OUTPUT  : int - Return Type
 REMARK  : 
**************************************************************************/
int TCC_DxB_CAS_Open(
	unsigned int algorithm,	   
	unsigned int operation_mode,
	unsigned int algorithm_mode,	
	unsigned int multi2Rounds 	
)
{
	int	ierr=0;
	TCC_CIPHER_ModeSettings CipherModeSetting;

	CipherModeSetting.algorithm = algorithm;
	CipherModeSetting.operation_mode = operation_mode;
	CipherModeSetting.algorithm_mode = algorithm_mode;
	CipherModeSetting.multi2Rounds = multi2Rounds;

	ierr = TSDEMUXDSCApi_Open(0, 0, 0, &CipherModeSetting);

	return ierr;
}

int TCC_DxB_CAS_OpenSWMulti2(
	unsigned int algorithm,	   
	unsigned int operation_mode,
	unsigned int algorithm_mode,	
	unsigned int multi2Rounds,
	unsigned char * sysKey
)
{
	int	ierr=0;

	TCCDEMUX_DESC_EncryptionSettings CipherModeSetting;

	CipherModeSetting.algorithmVar = 1;
	CipherModeSetting.bIsUsed = 1;
	CipherModeSetting.multi2KeySelect = 0;
	CipherModeSetting.multi2Rounds = multi2Rounds;
	memcpy(CipherModeSetting.multi2SysKey, sysKey, sizeof(CipherModeSetting.multi2SysKey));

	if(BCAS_MULTI2_SW_HANDLE != 0)
		TCC_DxB_CAS_CloseSWMulti2();
	ierr = TSDEMUXDSCApi_Open(&BCAS_MULTI2_SW_HANDLE, TCCMANAGER_DESC_MULTI2, (void*)&CipherModeSetting, 0);

	return ierr;
}

#if 1   //for HWDEMUX Cipher
int TCC_DxB_CAS_OpenHWDEMUX(
	unsigned int algorithm,	   
	unsigned int operation_mode,
	unsigned int algorithm_mode,	
	unsigned int multi2Rounds 	
)
{
	int	ierr=0;
	TCC_CIPHER_ModeSettings CipherModeSetting;

	CipherModeSetting.algorithm = algorithm;
	CipherModeSetting.operation_mode = operation_mode;
	CipherModeSetting.algorithm_mode = algorithm_mode;
	CipherModeSetting.multi2Rounds = multi2Rounds;

	ierr = TSDEMUXDSCApi_Open(0, 0, 0, &CipherModeSetting);

	return ierr;
}
#endif

/**************************************************************************
 FUNCTION NAME : TCC_DxB_CAS_Close
 
 DESCRIPTION : 
 
 INPUT	 : 
 OUTPUT  : int - Return Type
 REMARK  : 
**************************************************************************/
int TCC_DxB_CAS_Close(void)
{
	int	ierr =0;
	ierr = TSDEMUXDSCApi_Close(0);
	return ierr;
}

int TCC_DxB_CAS_CloseSWMulti2(void)
{
	int	ierr =0;
	ierr = TSDEMUXDSCApi_Close(BCAS_MULTI2_SW_HANDLE);
	if(ierr == 0)
		BCAS_MULTI2_SW_HANDLE = 0;
	return ierr;
}
/**************************************************************************
 FUNCTION NAME : TCC_DxB_CAS_SetKey
 
 DESCRIPTION : encryption or decryption �� ��� �� key �Է� 
 
 INPUT   : p_key_data - Parameter
           key_length - Parameter
           p_init_vector - Parameter
           p_key_data - Parameter
           vector_length - Parameter
 OUTPUT  : int - Return Type
 REMARK  : 
**************************************************************************/
int TCC_DxB_CAS_SetKey
(
	unsigned char *p_key_data,
	unsigned int key_length,
	unsigned char *p_init_vector,				// ������� �ʴ� ��� NULL
	unsigned int vector_length,				// ������� �ʴ� ��� '0'
	unsigned char *p_Systemkey_data,				// ������� �ʴ� ��� NULL
	unsigned int Systemkey_length,				// ������� �ʴ� ��� '0'
	unsigned int key_flag				// even or odd key
)
{
	int	ierr=0;
	TCC_CIPHER_KeySettings Keysetting;

	Keysetting.p_key_data = p_key_data;
	Keysetting.key_length = key_length;
	Keysetting.p_init_vector = p_init_vector;
	Keysetting.vector_length = vector_length;
	Keysetting.p_Systemkey_data = p_Systemkey_data;
	Keysetting.Systemkey_length = Systemkey_length;
	Keysetting.Key_flag = key_flag;

	ierr = TSDEMUXDSCApi_SetKey(0, 0, 0, 0, 0, 0, &Keysetting);

	return ierr;
}

int TCC_DxB_CAS_SetKeySWMulti2
(
	unsigned char parity,			//typedef enum {TCCDEMUX_DESC_ODD,	TCCDEMUX_DESC_EVEN } TCCDEMUX_DESC_ODD_EVEN;
	unsigned char *p_key_data,
	unsigned int key_length,
	unsigned char *p_init_vector,				// ������� �ʴ� ��� NULL
	unsigned int vector_length				// ������� �ʴ� ��� '0'
)
{
	int	ierr=0;
	int	piArg[5];

	ierr = TSDEMUXDSCApi_SetKey(BCAS_MULTI2_SW_HANDLE, parity, p_key_data, key_length, p_init_vector, vector_length, 0);

	return ierr;
}

