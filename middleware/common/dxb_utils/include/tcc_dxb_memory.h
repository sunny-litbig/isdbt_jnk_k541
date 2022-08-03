/****************************************************************************

Copyright (C) 2013 Telechips Inc.


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions 
andlimitations under the License.

****************************************************************************/

#ifndef	_TCC_DXB_MEMORY_H__
#define	_TCC_DXB_MEMORY_H__

/******************************************************************************
* include 
******************************************************************************/
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
* typedefs & structure
******************************************************************************/


/******************************************************************************
* defines 
******************************************************************************/


/******************************************************************************
* globals
******************************************************************************/


/******************************************************************************
* locals
******************************************************************************/


/******************************************************************************
* declarations
******************************************************************************/
void tcc_mw_memoryleakcheck_init(void);
void tcc_mw_memoryleakcheck_term(void);
void* tcc_mw_malloc(const char* functionName, unsigned int line, unsigned int size);
void tcc_mw_free(const char* functionName, unsigned int line, void* ptr);


#ifdef __cplusplus
}
#endif

#endif

