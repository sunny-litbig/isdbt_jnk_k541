/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_isdbtfseg_message.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef	_TCC_DXB_ISDBTFSEG_MESSAGE_H__
#define	_TCC_DXB_ISDBTFSEG_MESSAGE_H__

/******************************************************************************
* include 
******************************************************************************/
#include <pthread.h>
#include "tcc_message.h"

/******************************************************************************
* defines 
******************************************************************************/
extern TCC_MESSAGE_QUEUE_t	TCC_DXB_ISDBTFSEG_MESSAGE_ProcessMessageQueue;
extern TCC_MESSAGE_QUEUE_t	TCC_DXB_ISDBTFSEG_MESSAGE_UIMessageQueue;

void						TCC_DXB_ISDBTFSEG_MESSAGE_InitQueue(void);
TCC_MESSAGE_QUEUE_t*		TCC_DXB_ISDBTFSEG_MESSAGE_GetUIMessageQueue(void);
TCC_MESSAGE_QUEUE_t*		TCC_DXB_ISDBTFSEG_MESSAGE_GetProcessMessageQueue(void);

#endif
