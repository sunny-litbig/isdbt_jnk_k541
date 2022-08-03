/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_mainframe.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef __TCC_DXB_ISDBTFSEG_MAIN_H__
#define __TCC_DXB_ISDBTFSEG_MAIN_H__


/******************************************************************************
* include 
******************************************************************************/


/******************************************************************************
* defines 
******************************************************************************/
#define	_APPLICATION_VERSION_   "001"
#define _DXB_SERVICE_VER_        "DXB_SERVICE_VER_"_APPLICATION_VERSION_
#define _DXB_SERVICE_PATCH_VER_  _DXB_SERVICE_VER_""

/******************************************************************************
* typedefs & structure
******************************************************************************/

/******************************************************************************
* globals
******************************************************************************/
extern int Init_Video_Output_Path;
extern int Init_CountryCode;
extern int Init_ProcessMonitoring;
extern int Init_Support_UART_Console;
extern int Init_stand_alone_processing;
extern int Init_Not_Skip_Same_program;
extern int Init_Disable_DualDecode;
extern int Init_Mute_GPIO_Port;
extern int Init_Mute_GPIO_Polarity;
extern int Init_Support_PVR;
extern int Init_StrengthFullToOne;
extern int Init_StrengthOneToFull;
extern int Init_CountSecFullToOne;
extern int Init_CountSecOneToFull;
extern int Init_Support_PVR;
extern int Init_PVR_CountryCode_Mode;
extern int Init_PrintLog_SignalStrength;

/******************************************************************************
* locals
******************************************************************************/


/******************************************************************************
* declarations
******************************************************************************/


#endif
