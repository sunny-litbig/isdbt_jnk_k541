/******************************************************************************
* include 
******************************************************************************/
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include "tcc_semaphore.h"
#include <tcc_pthread_cond.h>

/******************************************************************************
* locals
******************************************************************************/




/******************************************************************************
*	FUNCTIONS			: 
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

void TCC_Sema_Init(TCC_Sema_t* Sema, unsigned int val) 
{
	  if(Sema == NULL)
	  	return;

	  tcc_pthread_cond_init(&Sema->condition, NULL);
	  pthread_mutex_init(&Sema->mutex, NULL);
	  Sema->value = val;
}

/******************************************************************************
*	FUNCTIONS			: 
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
void TCC_Sema_Deinit(TCC_Sema_t* Sema) 
{
	  pthread_cond_destroy(&Sema->condition);
	  pthread_mutex_destroy(&Sema->mutex);
}

/******************************************************************************
*	FUNCTIONS			: 
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
void TCC_Sema_Down(TCC_Sema_t* Sema) 
{
	  pthread_mutex_lock(&Sema->mutex);
	  while (Sema->value == 0)
	  {
		  pthread_cond_wait(&Sema->condition, &Sema->mutex);
	  }
	  Sema->value--;
	  pthread_mutex_unlock(&Sema->mutex);
}

int TCC_Sema_Down_TimeWait(TCC_Sema_t* Sema,int expire_time)
{
	int err = 0;
    struct timespec ts = { 0, };

    clock_gettime(CLOCK_MONOTONIC , &ts);
    ts.tv_sec += expire_time; 	// sec 단위로 입력..

	pthread_mutex_lock(&Sema->mutex);
	while (Sema->value == 0) {
		err = tcc_pthread_cond_timedwait(&Sema->condition, &Sema->mutex , &ts);

		if(err == ETIMEDOUT)
		{
			Sema->value++;
		}
	}
	Sema->value--;
	pthread_mutex_unlock(&Sema->mutex);

	if(err == ETIMEDOUT)
		return 0;
	else return 1;
}




/******************************************************************************
*	FUNCTIONS			: 
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/

void TCC_Sema_Up(TCC_Sema_t* Sema) 
{
	  pthread_mutex_lock(&Sema->mutex);
	  Sema->value++;
	  pthread_cond_signal(&Sema->condition);
	  pthread_mutex_unlock(&Sema->mutex);
}

/******************************************************************************
*	FUNCTIONS			: 
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
void TCC_Sema_Reset(TCC_Sema_t* Sema) 
{
	  pthread_mutex_lock(&Sema->mutex);
	  Sema->value=0;
	  pthread_mutex_unlock(&Sema->mutex);
}

/******************************************************************************
*	FUNCTIONS			: 
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
void TCC_Sema_Wait(TCC_Sema_t* Sema) 
{
	  pthread_mutex_lock(&Sema->mutex);
	  pthread_cond_wait(&Sema->condition, &Sema->mutex);
	  pthread_mutex_unlock(&Sema->mutex);
}

/******************************************************************************
*	FUNCTIONS			: 
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
void TCC_Sema_Signal(TCC_Sema_t* Sema) 
{
	  pthread_mutex_lock(&Sema->mutex);
	  pthread_cond_signal(&Sema->condition);
	  pthread_mutex_unlock(&Sema->mutex);
}

/******************************************************************************
*	FUNCTIONS			: 
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
void TCC_Sema_Lock(TCC_Sema_t* Sema) 
{
	  pthread_mutex_lock(&Sema->mutex);
}

/******************************************************************************
*	FUNCTIONS			: 
*	SYNOPSIS			:
*	EXTERNAL EFFECTS	:
*	PARAMETERS			:
*	RETURNS				:
*	ERRNO				:
******************************************************************************/
void TCC_Sema_Unlock(TCC_Sema_t* Sema) 
{
	  pthread_mutex_unlock(&Sema->mutex);
}

