
/****************************************************************************
 *   FileName    :  tcc_memory_debug.h
 *   Description : 
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips, Inc.
 *   ALL RIGHTS RESERVED
 *
 ****************************************************************************/


#ifndef	_TCC_MEMORY_DEBUG_H_
#define	_TCC_MEMORY_DEBUG_H_

/*****************************************************************************
*
* Include
*
******************************************************************************/
#include "Tc_linkedlist.h"

/*****************************************************************************
*
* Type Defines
*
******************************************************************************/

/*****************************************************************************
*
* Structures
*
******************************************************************************/
typedef struct TCC_MEMORY_structure
{
	unsigned int 	m_uiMem_Size;
	void 			*m_pStartAdd;
}TCC_MEMORY_INFO_t;

typedef struct TCC_DEBUG_MEMORY_structure
{
	DLinked_list 		m_DLList;
	pthread_mutex_t mutex;
	unsigned int 		m_uiMem_Counter;
	unsigned int 		m_uiMem_CurSize;
	unsigned int 		m_uiMem_MaxPitch;

}TCC_DEBUG_MEM_INFO_t;

void TCC_Debug_Display_Memory_Info( void);
void TCC_Debug_Display_Memory_Leak( void);
void TCC_Debug_Memory_Info_Insert( void *pStartAdd, unsigned int uiSize);
void TCC_Debug_Memory_Info_Remove( void *pStartAdd);
void TCC_Debug_Display_Memory_Info( void);

#endif /* _TCC_MEMORY_DEBUG_H_ */
