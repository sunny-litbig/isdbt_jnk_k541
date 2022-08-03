/**

  Copyright (C) 2009-2010 Telechips Inc.

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA
  02110-1301  USA

*/

#ifndef	_TCC_MEMORY_H__
#define	_TCC_MEMORY_H__

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
void tcc_dxb_fo_MemoryLeakCheck_clear(void);
void tcc_dxb_fo_MemoryLeakCheck_init(void);
void tcc_dxb_fo_MemoryLeakCheck_term(void);
void* TCC_fo_calloc(const char* functionName, unsigned int line, unsigned int isize_t, unsigned int iSize);
void* TCC_fo_malloc(const char* functionName, unsigned int line, unsigned int iSize);
void TCC_fo_free(const char* functionName, unsigned int line, void *pvPtr);



#ifdef __cplusplus
}
#endif

#endif //_TCC_UTIL_H___
