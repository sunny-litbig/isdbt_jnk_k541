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
#ifndef	_TCC_MALLOC_HOOKING__H__
#define	_TCC_MALLOC_HOOKING__H__

/******************************************************************************
* include 
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
* defines 
******************************************************************************/
#define MAX_TCC_MALLOC_HOOK_INFO_SLOTS			32768
#define MAX_TCC_MALLOC_HOOK_SUMMARY_CALLER		1024

#define FLAG_TCC_MALLOC_HOOK_STATUS_FREE		0x00000000
#define FLAG_TCC_MALLOC_HOOK_STATUS_USED		0x00000001

/******************************************************************************
* typedefs & structure
******************************************************************************/
typedef struct tag_TCC_MALLOC_HOOK_INFO
{
	int		flag;
	void*	ptr;
	int		size;
	void*	caller;
} TCC_MALLOC_HOOK_INFO;

typedef struct tag_TCC_MALLOC_HOOK_SUMMARY_CALLER
{
	int		flag;
	void*	caller;
	int		total_used_slots;
	int		total_used_size;
} TCC_MALLOC_HOOK_SUMMARY_CALLER;

/******************************************************************************
* globals
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

extern void tcc_malloc_hookinfo_init();
extern void tcc_malloc_hookinfo_update_malloc(void *ptr, int size, void* caller);
extern void tcc_malloc_hookinfo_update_realloc(void *ptr, void *oldptr, int size, void* caller);
extern void tcc_malloc_hookinfo_update_free(void *ptr, void* caller);
extern void tcc_malloc_hookinfo_proc_command(int mode, int param1, int param2, int param3, int param4);

#ifdef __cplusplus
}
#endif

/******************************************************************************
* declarations
******************************************************************************/


#endif //_TCC_UTIL_H___
