/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_dxb_isdbtfseg_cmd.c
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/

/******************************************************************************
* include 
******************************************************************************/
#include "tcc_dxb_isdbtfseg_cmd.h"
#include "tcc_dxb_isdbtfseg_message.h"

/******************************************************************************
* globals
******************************************************************************/
TCC_DXB_ISDBTFSEG_CMD_t	TCC_DXB_ISDBTFSEG_CMD_Cmd;

/******************************************************************************
* Funtions
******************************************************************************/
void TCC_DXB_ISDBTFSEG_CMD_InitCmd(void)
{
	TCC_DXB_ISDBTFSEG_CMD_Cmd.Opcode				= TCC_OPCODE_NULL;
	TCC_DXB_ISDBTFSEG_CMD_Cmd.pUIMessageQueue		= TCC_DXB_ISDBTFSEG_MESSAGE_GetUIMessageQueue();
	TCC_DXB_ISDBTFSEG_CMD_Cmd.pProcessMessageQueue	= TCC_DXB_ISDBTFSEG_MESSAGE_GetProcessMessageQueue();
	
	TCC_DXB_ISDBTFSEG_CMD_Cmd.pCmdSem = TCC_malloc(sizeof(TCC_Sema_t));
	TCC_Sema_Init(TCC_DXB_ISDBTFSEG_CMD_Cmd.pCmdSem, 0);
}

void TCC_DXB_ISDBTFSEG_CMD_DeinitCmd(void)
{
	TCC_DXB_ISDBTFSEG_CMD_Cmd.Opcode				= TCC_OPCODE_NULL;
	TCC_DXB_ISDBTFSEG_CMD_Cmd.pUIMessageQueue		= NULL;
	TCC_DXB_ISDBTFSEG_CMD_Cmd.pProcessMessageQueue	= NULL;

	TCC_Sema_Deinit(TCC_DXB_ISDBTFSEG_CMD_Cmd.pCmdSem);
	TCC_free(TCC_DXB_ISDBTFSEG_CMD_Cmd.pCmdSem);
}

void TCC_DXB_ISDBTFSEG_CMD_SetOpcode(void)
{
	TCC_DXB_ISDBTFSEG_CMD_Cmd.Opcode = TCC_OPCODE_DXB_ISDBTFSEG_DEC;
}

int TCC_DXB_ISDBTFSEG_CMD_GetOpcode(void)
{
	return(TCC_DXB_ISDBTFSEG_CMD_Cmd.Opcode);
}

TCC_MESSAGE_t* TCC_DXB_ISDBTFSEG_Set_Process_Message( int iOperand, int iCmdType, int iFreeFlag, int iDataSize, void *pvData, void (*donfunction)(void *))
{
	TCC_MESSAGE_t *pProcessMessage = TCC_Creat_Message();
	if(pProcessMessage == NULL)
	{
		return NULL;
	}

	pProcessMessage->Opcode 		=  TCC_DXB_ISDBTFSEG_CMD_GetOpcode();//TCC_DXB_ISDBTFSEG_CMD_Cmd.Opcode;
	//printf("%s %d %d\n", __func__, __LINE__, TCC_DXB_ISDBTFSEG_CMD_Cmd.Opcode);
	//printf("%s %d %d\n", __func__, __LINE__, pProcessMessage->Opcode);
	pProcessMessage->Operand 		= iOperand;
	pProcessMessage->Cmd_Type 		= iCmdType;
	pProcessMessage->Free_Flag 		= iFreeFlag;
	pProcessMessage->Data_Size 		= iDataSize;
	pProcessMessage->Data 			= pvData;
	pProcessMessage->DonFunction 	= donfunction;

	return pProcessMessage;
}

TCC_MESSAGE_t* TCC_DXB_ISDBTFSEG_CMD_SetUIMessage( int iOperand, int iCmdType, int iFreeFlag, int iDataSize, void *pvData, void (*donfunction)(void *))
{
	TCC_MESSAGE_t *pUIMessage = TCC_Creat_Message();
	if(pUIMessage == NULL)
	{
		return NULL;
	}

	pUIMessage->Opcode 			= TCC_DXB_ISDBTFSEG_CMD_GetOpcode();//TCC_DXB_ISDBTFSEG_CMD_Cmd.Opcode;
	//printf("%s %d %d\n", __func__, __LINE__, pUIMessage->Opcode);	
	pUIMessage->Operand 		= iOperand;
	pUIMessage->Cmd_Type 		= iCmdType;
	pUIMessage->Free_Flag 		= iFreeFlag;
	pUIMessage->Data_Size 		= iDataSize;
	pUIMessage->Data 			= pvData;
	pUIMessage->DonFunction 	= donfunction;

	return pUIMessage;
}

TCC_MESSAGE_t* TCC_DXB_ISDBTFSEG_CMD_GetProcessMessage(void)
{
	return(TCC_Get_Message(TCC_DXB_ISDBTFSEG_CMD_Cmd.pProcessMessageQueue));
}

TCC_MESSAGE_t* TCC_DXB_ISDBTFSEG_CMD_GetUIMessage(void)
{
	return(TCC_Get_Message(TCC_DXB_ISDBTFSEG_CMD_Cmd.pUIMessageQueue));
}

int TCC_DXB_ISDBTFSEG_CMD_GetProcessMessageCount(void)
{
	return(TCC_Get_Message_Count(TCC_DXB_ISDBTFSEG_CMD_Cmd.pProcessMessageQueue));
}

int TCC_DXB_ISDBTFSEG_CMD_GetUIMessageCount(void)
{
	return(TCC_Get_Message_Count(TCC_DXB_ISDBTFSEG_CMD_Cmd.pUIMessageQueue));
}
