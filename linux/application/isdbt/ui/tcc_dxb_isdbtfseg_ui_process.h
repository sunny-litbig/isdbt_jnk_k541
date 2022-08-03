/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_isdbtfseg_ui_process.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef __TCC_DXB_ISDBTFSEG_UI_PROCESS_H__
#define __TCC_DXB_ISDBTFSEG_UI_PROCESS_H__

/******************************************************************************
* typedefs & structure
******************************************************************************/
typedef struct tag_TCC_DXB_ISDBTFSEG_UI_PROCESS_Instance_T
{
	int			iRunFlag;
	pthread_t	tThread;
} TCC_DXB_ISDBTFSEG_UI_PROCESS_Instance_T;


/******************************************************************************
* declarations
******************************************************************************/
int	TCC_DXB_ISDBTFSEG_UI_PROCESS_Init(void);
int TCC_DXB_ISDBTFSEG_UI_PROCESS_DeInit(void);

#endif

