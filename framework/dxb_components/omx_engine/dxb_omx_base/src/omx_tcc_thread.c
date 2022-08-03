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

/****************************************************************************
*   FileName    : omx_tcc_thread.c
*   Description : 
****************************************************************************
*
*   TCC Version 1.0
*   Copyright (c) Telechips, Inc.
*   ALL RIGHTS RESERVED
*
****************************************************************************/

/****************************************************************************

Revision History

****************************************************************************

****************************************************************************/
#define LOG_TAG	"TCC_THREAD"

#include <pthread.h>
#include <stdlib.h>
#include <utils/Log.h>
#include "omx_tcc_thread.h"

//#define	 NOPRIORITY_SUPPORT
#define	 DBG_PRINTF		ALOGD

typedef struct TCCTHREAD_COUSTOM_DEF
{
	unsigned char *pucname;
	int sched_type;
	E_TCCTHREAD_PRIORITY epriority;
} ST_TCCTHREAD_COUSTOM_DEF;


ST_TCCTHREAD_COUSTOM_DEF stTCCThreadTable[] = 
{
	{	"default"      ,					SCHED_RR,	LOW_PRIORITY_11		},		
	{	"OMX.tcc.broadcast.tuner.atsc",		SCHED_RR,	LOW_PRIORITY_11		},
	{	"OMX.tcc.broadcast.dvbt",			SCHED_RR,	LOW_PRIORITY_11		},
	{	"OMX.tcc.broadcast.isdbt",			SCHED_RR,	LOW_PRIORITY_11		},	
	{	"OMX.tcc.broadcast.demux.atsc",		SCHED_FIFO,	HIGH_PRIORITY_0		},
	{	"OMX.tcc.broadcast.dxb_demux",		SCHED_FIFO,	HIGH_PRIORITY_0		},
	{	"OMX.tcc.broadcast.iptv_demux",		SCHED_FIFO,	HIGH_PRIORITY_0		},
	{	"OMX.tcc.broadcast.tdmb_demux",		SCHED_FIFO,	HIGH_PRIORITY_0		},
	{	"OMX.tcc.broadcast.tdmb_padparser",	SCHED_FIFO,	HIGH_PRIORITY_0		},
	{	"OMX.tcc.broadcast.section",		SCHED_RR,	HIGH_PRIORITY_7		},
	{	"OMX.tcc.video_decoder",			SCHED_RR,	HIGH_PRIORITY_7		},
	{	"OMX.tcc.audio_decoder",			SCHED_RR,	HIGH_PRIORITY_7		},
	{	"OMX.TCC.mpeg2dec",					SCHED_RR,	HIGH_PRIORITY_7		},
	{	"OMX.TCC.avcdec",					SCHED_RR,	HIGH_PRIORITY_7		},
	{	"OMX.TCC.mp2dec",					SCHED_RR,	HIGH_PRIORITY_7		},
	{	"OMX.TCC.aacdec",					SCHED_RR,	HIGH_PRIORITY_7		},
	{	"OMX.TCC.ac3dec",					SCHED_RR,	HIGH_PRIORITY_7		},
	{	"OMX.tcc.audio_decoder.bsac_dec",	SCHED_RR,	HIGH_PRIORITY_7		},
	{	"OMX.tcc.fbdev.fbdev_sink",		SCHED_RR,	HIGH_PRIORITY_0		},
	{	"OMX.tcc.alsa.alsasink",		SCHED_RR,	HIGH_PRIORITY_0		},
	{	NULL,								0,			0 					},
};


int TCCTHREAD_Create(void *pHandle, pThreadFunc Func, char *pcName, E_TCCTHREAD_PRIORITY ePriority, void *arg)
{
	int i, ret = 0;
	int iSchedType, iPriorityOffset, iMaxPriority;
	struct sched_param param;
	pthread_attr_t p_tattr;
	
#ifdef		NOPRIORITY_SUPPORT
	return pthread_create(pHandle, NULL, Func,  arg);
#else

	iSchedType = -1;
	if( ePriority >= PREDEF_PRIORITY )
		ePriority = HIGH_PRIORITY_9;

	if(pcName)
	{		
		for( i=0; stTCCThreadTable[i].pucname != NULL; i++ )
		{
			if( !strcmp(stTCCThreadTable[i].pucname, pcName) )
			{				
				iPriorityOffset = stTCCThreadTable[i].epriority;	
				iSchedType = stTCCThreadTable[i].sched_type;	
				DBG_PRINTF("%s %d %d", pcName, ePriority, iSchedType);
				break;
			}
		}
	}else{
		return -1;
	}
	
	if(iSchedType == -1)
	{
		if(ePriority >= LOW_PRIORITY_0 )
		{
			#if 1
			iSchedType = SCHED_OTHER;		
			//iPriorityOffset = ePriority-LOW_PRIORITY_0;		
			iPriorityOffset = 0;
			#else
			iSchedType = SCHED_RR;
			iPriorityOffset = ePriority;
			#endif
		}
		else
		{
			iSchedType = SCHED_RR;
			iPriorityOffset = ePriority;
		}	
	}	
	/* initialized with default attributes */
	ret |= pthread_attr_init (&p_tattr);
	
	//ret |= pthread_attr_setinheritsched(p_tattr, PTHREAD_EXPLICIT_SCHED);  
	ret |= pthread_attr_setscope(&p_tattr, PTHREAD_SCOPE_SYSTEM);	
	ret |= pthread_attr_setschedpolicy(&p_tattr, iSchedType);
	/* safe to get existing scheduling param */
	ret |= pthread_attr_getschedparam (&p_tattr, &param);
	
	/* set the priority; others are unchanged */
	iMaxPriority = sched_get_priority_max(iSchedType);
	if( iMaxPriority > iPriorityOffset )
		param.sched_priority = iMaxPriority - iPriorityOffset;
	else
		param.sched_priority = iMaxPriority;
	//ALOGE("MAX %d,Offset %d, Type %d", sched_get_priority_max(iSchedType), iPriorityOffset, iSchedType);
	/* setting the new scheduling param */
	ret |= pthread_attr_setschedparam (&p_tattr, &param);
	if(ret != 0)
	{
		DBG_PRINTF("%s::%d thread(%s) error %d", __func__,__LINE__, pcName, ret);
		return ret;
	}
	
	ret = pthread_create(pHandle, &p_tattr, Func,  arg);	
	if(ret != 0)
	{
		DBG_PRINTF("%s::%d thread(%s) error %d", __func__,__LINE__, pcName, ret);
		return ret;
	}
	ret = pthread_attr_destroy(&p_tattr);
	if(ret != 0)
	{
		DBG_PRINTF("%s::%d thread(%s) error %d", __func__,__LINE__, pcName, ret);
		return ret;
	}
	
	i = strlen(pcName);
	if (i > 0) {
		char *pName = pcName;
		if (i > 15)
			pName += (i-15);
		pthread_setname_np (*(pthread_t *)pHandle, pName);
	}
	
	DBG_PRINTF("%s::%s Success!! Priority(%d)Type(%d)", __func__, pcName, param.sched_priority, iSchedType);
	return ret;
#endif	
}

int TCCTHREAD_Join(void *pHandle, void **pThreadRet)
{
	return pthread_join(pHandle, pThreadRet);
}
