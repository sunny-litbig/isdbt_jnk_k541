/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : TCCMemory.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef	_TCC_MEMORY_H__
#define	_TCC_MEMORY_H__

/******************************************************************************
* include 
******************************************************************************/
#include <stdlib.h>

/******************************************************************************
* typedefs & structure
******************************************************************************/


/******************************************************************************
* defines 
******************************************************************************/

#define	TCC_LINUX_MEMORY_SYTEM

//#define	TCC_MEMORY_DEBUG	// for Telechips Memory  Debugging 


/******************************************************************************
* globals
******************************************************************************/

/******************************************************************************
* locals
******************************************************************************/


/******************************************************************************
* declarations
******************************************************************************/
void* TCC_malloc(unsigned int iSize);
void* TCC_calloc(unsigned int isize_t, unsigned int iSize);
void* TCC_realloc(void *p,unsigned int iSize);
int TCC_free(void *pvPtr);

#endif //_TCC_UTIL_H___
