/******************************************************************************
*
*  (C)Copyright All Rights Reserved by Telechips Inc.
*                                   
*  This material is confidential and shall remain as such. 
*  Any unauthorized use, distribution, reproduction is strictly prohibited. 
*
*   FileName    : tcc_semaphore.h
*   Description : 
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*******************************************************************************/
#ifndef	_TCC_SEMAPHORE_H__
#define	_TCC_SEMAPHORE_H__

/******************************************************************************
* include 
******************************************************************************/
#include <pthread.h>


/******************************************************************************
* defines 
******************************************************************************/


/******************************************************************************
* typedefs & structure
******************************************************************************/
#ifndef 	TRUE
#define	TRUE 	1
#define	FALSE	0
#endif


typedef struct TCC_SEMAPHORE_t
{
  pthread_cond_t condition;
  pthread_mutex_t mutex;
  unsigned int value;
}TCC_Sema_t,*pTCC_Sema_t;



/******************************************************************************
* globals
******************************************************************************/



/******************************************************************************
* locals
******************************************************************************/


/******************************************************************************
* declarations
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void TCC_Sema_Init(TCC_Sema_t* Sema, unsigned int val) ;
void TCC_Sema_Deinit(TCC_Sema_t* Sema) ;
void TCC_Sema_Down(TCC_Sema_t* Sema) ;
void TCC_Sema_Up(TCC_Sema_t* Sema) ;
void TCC_Sema_Reset(TCC_Sema_t* Sema) ;
void TCC_Sema_Wait(TCC_Sema_t* Sema) ;
void TCC_Sema_Signal(TCC_Sema_t* Sema); 
void TCC_Sema_Lock(TCC_Sema_t* Sema);
void TCC_Sema_Unlock(TCC_Sema_t* Sema);
int TCC_Sema_Down_TimeWait(TCC_Sema_t* Sema,int expire_time);


#ifdef __cplusplus
}
#endif



#endif //_TCC_SEMAPHORE_H__




