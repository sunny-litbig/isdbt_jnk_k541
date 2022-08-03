/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_dxb_isdbtfseg_cmd.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef __TCC_DXB_ISDBTFSEG_CMD_H__
#define __TCC_DXB_ISDBTFSEG_CMD_H__

/******************************************************************************
* include 
******************************************************************************/
#include <tcc_message.h>
#include <tcc_common_cmd.h>
#include <tcc_semaphore.h>

/******************************************************************************
* typedefs & structure
******************************************************************************/
typedef	struct TCC_DXB_ISDBTFSEG_CMD_Structure
{
	int						Opcode;
	TCC_MESSAGE_QUEUE_t*	pUIMessageQueue;
	TCC_MESSAGE_QUEUE_t*	pProcessMessageQueue;
	pTCC_Sema_t*			pCmdSem;
}TCC_DXB_ISDBTFSEG_CMD_t;

/******************************************************************************
* globals
******************************************************************************/


/******************************************************************************
* locals
******************************************************************************/
extern TCC_DXB_ISDBTFSEG_CMD_t	TCC_DXB_ISDBTFSEG_CMD_Cmd;


/******************************************************************************
* declarations
******************************************************************************/
void			TCC_DXB_ISDBTFSEG_CMD_InitCmd(void);
void			TCC_DXB_ISDBTFSEG_CMD_DeinitCmd(void);
void			TCC_DXB_ISDBTFSEG_CMD_SetOpcode(void);
int				TCC_DXB_ISDBTFSEG_CMD_GetOpcode(void);
TCC_MESSAGE_t*	TCC_DXB_ISDBTFSEG_Set_Process_Message(int operand, int cmd_type,int free_flag, int data_size, void* data, void (*donfunction)(void *));
TCC_MESSAGE_t*	TCC_DXB_ISDBTFSEG_CMD_SetUIMessage(int operand, int cmd_type, int free_flag, int data_size, void* data, void (*donfunction)(void *));
TCC_MESSAGE_t*	TCC_DXB_ISDBTFSEG_CMD_GetProcessMessage(void);
TCC_MESSAGE_t*	TCC_DXB_ISDBTFSEG_CMD_GetUIMessage(void);
int				TCC_DXB_ISDBTFSEG_CMD_GetProcessMessageCount(void);
int				TCC_DXB_ISDBTFSEG_CMD_GetUIMessageCount(void);

#endif
