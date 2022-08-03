/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_dxb_isdbtfseg_message.c
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/

/******************************************************************************
* include 
******************************************************************************/
#include "tcc_dxb_isdbtfseg_message.h"


/******************************************************************************
* globals
******************************************************************************/
TCC_MESSAGE_QUEUE_t	TCC_DXB_ISDBTFSEG_MESSAGE_ProcessMessageQueue;
TCC_MESSAGE_QUEUE_t	TCC_DXB_ISDBTFSEG_MESSAGE_UIMessageQueue;

/******************************************************************************
* Funtions
******************************************************************************/
void TCC_DXB_ISDBTFSEG_MESSAGE_InitQueue(void)
{
	// Init Process message	
	pthread_mutex_init(&TCC_DXB_ISDBTFSEG_MESSAGE_ProcessMessageQueue.mutex, NULL);
	TCC_DXB_ISDBTFSEG_MESSAGE_ProcessMessageQueue.message_count	= 0;
	TCC_DXB_ISDBTFSEG_MESSAGE_ProcessMessageQueue.message_type	= MESSAGE_TYPE_UI_TO_PROCESS;
	TCC_DXB_ISDBTFSEG_MESSAGE_ProcessMessageQueue.message_data	= NULL;
	TCC_DXB_ISDBTFSEG_MESSAGE_ProcessMessageQueue.header		= TCC_DXB_ISDBTFSEG_MESSAGE_ProcessMessageQueue.message_data;
	TCC_DXB_ISDBTFSEG_MESSAGE_ProcessMessageQueue.tail			= TCC_DXB_ISDBTFSEG_MESSAGE_ProcessMessageQueue.message_data;	

	// Init UI message
	pthread_mutex_init(&TCC_DXB_ISDBTFSEG_MESSAGE_UIMessageQueue.mutex, NULL);	
	TCC_DXB_ISDBTFSEG_MESSAGE_UIMessageQueue.message_count		= 0;
	TCC_DXB_ISDBTFSEG_MESSAGE_UIMessageQueue.message_type		= MESSAGE_TYPE_PROCESS_TO_UI;
	TCC_DXB_ISDBTFSEG_MESSAGE_UIMessageQueue.message_data		= NULL;
	TCC_DXB_ISDBTFSEG_MESSAGE_UIMessageQueue.header				= TCC_DXB_ISDBTFSEG_MESSAGE_UIMessageQueue.message_data;
	TCC_DXB_ISDBTFSEG_MESSAGE_UIMessageQueue.tail				= TCC_DXB_ISDBTFSEG_MESSAGE_UIMessageQueue.message_data;
}

TCC_MESSAGE_QUEUE_t* TCC_DXB_ISDBTFSEG_MESSAGE_GetUIMessageQueue(void)
{
	return(&TCC_DXB_ISDBTFSEG_MESSAGE_UIMessageQueue);
}

TCC_MESSAGE_QUEUE_t* TCC_DXB_ISDBTFSEG_MESSAGE_GetProcessMessageQueue(void)
{
	return(&TCC_DXB_ISDBTFSEG_MESSAGE_ProcessMessageQueue);
}
