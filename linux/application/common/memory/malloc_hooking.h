/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef	_MALLOC_HOOKING__H__
#define	_MALLOC_HOOKING__H__

/******************************************************************************
* include 
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/******************************************************************************
* defines 
******************************************************************************/
#define MAX_MALLOC_HOOK_INFO_SLOTS			32768
#define MAX_MALLOC_HOOK_SUMMARY_CALLER		8192

#define FLAG_MALLOC_HOOK_STATUS_FREE		0x00000000
#define FLAG_MALLOC_HOOK_STATUS_USED		0x00000001

#define DELAY_US_MALLOC_HOOK_NO_PRINT		10

/******************************************************************************
* typedefs & structure
******************************************************************************/
typedef struct tag_MALLOC_HOOK_INFO
{
	int		flag;
	void*	ptr;
	int		size;
	void*	caller;
} MALLOC_HOOK_INFO;

typedef struct tag_MALLOC_HOOK_SUMMARY_CALLER
{
	int		flag;
	void*	caller;
	int		total_used_slots;
	int		total_used_size;
} MALLOC_HOOK_SUMMARY_CALLER;

/******************************************************************************
* globals
******************************************************************************/
extern void my_malloc_hookinfo_proc_command(int mode, int param1, int param2, int param3, int param4);


/******************************************************************************
* declarations
******************************************************************************/


#endif //_TCC_UTIL_H___
