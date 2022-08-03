/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_dxb_isdbtfseg_cui.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef __TCC_DXB_ISDBTFSEG_CUI_H__
#define __TCC_DXB_ISDBTFSEG_CUI_H__


/******************************************************************************
* declarations
******************************************************************************/
void	TCC_DXB_ISDBTFSEG_CUI_Init(void);
void	TCC_DXB_ISDBTFSEG_CUI_DeInit(void);
int		TCC_DXB_ISDBTFSEG_CUI_ProcMessage(void);
int		TCC_DXB_ISDBTFSEG_CUI_ParseMessage(char *pacBuffer);

#endif

