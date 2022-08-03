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

/******************************************************************************
* include
******************************************************************************/
#include "tcc_dxb_memory.h"
#define LOG_TAG	"tcc_dxb_memory"
#include <utils/Log.h>


//#define TCC_MW_MEMORYLEAKCHECK


#ifdef TCC_MW_MEMORYLEAKCHECK

#include <pthread.h>
#include <tcc_dxb_thread.h>

#define	TCC_MW_MEMORYLEAKCHECK_STRUCT_MAX       256*1024
#define TCC_MW_MEMORYLEAKCHECK_FILE_NAME_MAX    48

typedef struct _TccMw_MemoryLeakCheck_Struct{
	char     functionName[TCC_MW_MEMORYLEAKCHECK_FILE_NAME_MAX];
	int      line;
	int      size;
	int      usage;
	void*    ptr;
}TccMw_MemoryLeakCheck_Struct;
static TccMw_MemoryLeakCheck_Struct g_TccMw_MemoryLeakCheck_Struct[TCC_MW_MEMORYLEAKCHECK_STRUCT_MAX];
static unsigned int g_TccMw_MemoryLeakCheck_CurrentTotalMallocSize = 0;
static unsigned int g_TccMw_MemoryLeakCheck_CurrentTotalMallocCount = 0;
static pthread_mutex_t g_TccMw_MemoryLeakCheck_Mutex;

static unsigned int g_TccMw_MemoryLeakCheck_HighestIndex = 0;    // Highest index of the TccMw_MemoryLeakCheck_Struct while checking memory leak


////////////////////////////////////////////////////////////////////
// Thread, to check current memory usage every 10 seconds

static pthread_t g_TccMw_MemoryLeakCheck_ThreadId;
static int g_TccMw_MemoryLeakCheck_ThreadRunning = 0;

void* tcc_mw_memoryleakcheck_thread(void *arg)
{
	int i = 0;

	while(g_TccMw_MemoryLeakCheck_ThreadRunning)
	{
		printf("[%s] CurrentTotalMallocSize(%d), CurrentTotalMallocCount(%d)\n",
			__FUNCTION__, g_TccMw_MemoryLeakCheck_CurrentTotalMallocSize, g_TccMw_MemoryLeakCheck_CurrentTotalMallocCount);

		for(i = 0; i < 1000; i++)    // 10 seconds ( 10 ms * 1000)
//		for(i = 0; i < 100; i++)    // 1 second ( 10 ms * 100)		
		{
			if(0 == g_TccMw_MemoryLeakCheck_ThreadRunning)
				break;

			usleep(10 * 1000);
		}
	}

	g_TccMw_MemoryLeakCheck_ThreadRunning = -1;

	return (void*)NULL; 
}

// Thread
////////////////////////////////////////////////////////////////////

#endif

void tcc_mw_memoryleakcheck_init(void)
{
#ifdef TCC_MW_MEMORYLEAKCHECK

	int i = 0;

	printf("[%s] start\n", __FUNCTION__);

	pthread_mutex_init(&g_TccMw_MemoryLeakCheck_Mutex, 0);

	for(i = 0; i < TCC_MW_MEMORYLEAKCHECK_STRUCT_MAX; i++)
	{
		g_TccMw_MemoryLeakCheck_Struct[i].usage = 0;
	}

	g_TccMw_MemoryLeakCheck_CurrentTotalMallocSize = 0;
	g_TccMw_MemoryLeakCheck_CurrentTotalMallocCount = 0;

	g_TccMw_MemoryLeakCheck_ThreadRunning = 1;
	tcc_dxb_thread_create((void *)&g_TccMw_MemoryLeakCheck_ThreadId, 
							tcc_mw_memoryleakcheck_thread, 
							(unsigned char*)"TccMw_MemoryLeakCheck_thread", 
							HIGH_PRIORITY_2, //LOW_PRIORITY_11, 
							NULL);

#endif
}

void tcc_mw_memoryleakcheck_term(void)
{
#ifdef TCC_MW_MEMORYLEAKCHECK

	int i = 0;
	int memoryLeakCount = 0;
	int totalMemoryLeak = 0;

	g_TccMw_MemoryLeakCheck_ThreadRunning = 0;
	while(1)
	{
		if(g_TccMw_MemoryLeakCheck_ThreadRunning == -1)
			break;

		usleep(5 * 1000);
	}

	pthread_mutex_lock(&g_TccMw_MemoryLeakCheck_Mutex);
	
	for(i = 0; i < TCC_MW_MEMORYLEAKCHECK_STRUCT_MAX; i++)
	{
		if (g_TccMw_MemoryLeakCheck_Struct[i].usage == 1)
		{
			memoryLeakCount++;
			printf("[%s] Memory Leak %d / ptr(0x%x), function(%s), line(%d), size(%d)\n", 
				__FUNCTION__, memoryLeakCount,
				g_TccMw_MemoryLeakCheck_Struct[i].ptr, g_TccMw_MemoryLeakCheck_Struct[i].functionName,
				g_TccMw_MemoryLeakCheck_Struct[i].line, g_TccMw_MemoryLeakCheck_Struct[i].size);

			totalMemoryLeak += g_TccMw_MemoryLeakCheck_Struct[i].size;
		}
	}

	printf("[%s] totalMemoryLeak(%d), Max(%d), g_TccMw_MemoryLeakCheck_HighestIndex(%d)\n",
		__FUNCTION__, totalMemoryLeak, TCC_MW_MEMORYLEAKCHECK_STRUCT_MAX, g_TccMw_MemoryLeakCheck_HighestIndex);

	pthread_mutex_unlock(&g_TccMw_MemoryLeakCheck_Mutex);
	pthread_mutex_destroy(&g_TccMw_MemoryLeakCheck_Mutex);

#endif
}

void* tcc_mw_malloc(const char* functionName, unsigned int line, unsigned int size)
{
	int i = 0;
	void *ptr = NULL;

	ptr = malloc(size);

#ifdef TCC_MW_MEMORYLEAKCHECK

	pthread_mutex_lock(&g_TccMw_MemoryLeakCheck_Mutex);

	for(i = 0; i < TCC_MW_MEMORYLEAKCHECK_STRUCT_MAX; i++)
	{
		if(g_TccMw_MemoryLeakCheck_Struct[i].usage == 0)
		{
			strncpy(g_TccMw_MemoryLeakCheck_Struct[i].functionName, functionName, TCC_MW_MEMORYLEAKCHECK_FILE_NAME_MAX);
			g_TccMw_MemoryLeakCheck_Struct[i].functionName[TCC_MW_MEMORYLEAKCHECK_FILE_NAME_MAX - 1] = 0; /* null terminated */
			g_TccMw_MemoryLeakCheck_Struct[i].line = line;
			g_TccMw_MemoryLeakCheck_Struct[i].size = size;
			g_TccMw_MemoryLeakCheck_Struct[i].ptr = ptr;
			g_TccMw_MemoryLeakCheck_Struct[i].usage = 1;

			g_TccMw_MemoryLeakCheck_CurrentTotalMallocSize += size;
			g_TccMw_MemoryLeakCheck_CurrentTotalMallocCount++;

			if(g_TccMw_MemoryLeakCheck_HighestIndex < i)
				g_TccMw_MemoryLeakCheck_HighestIndex = i;

			break;
		}
	}

	if (i == TCC_MW_MEMORYLEAKCHECK_STRUCT_MAX)
		printf("[%s] malloc / Critical Error Happened / Please check this log !!!!!\n", __FUNCTION__);

	pthread_mutex_unlock(&g_TccMw_MemoryLeakCheck_Mutex);

#endif

	return ptr;
}

void tcc_mw_free(const char* functionName, unsigned int line, void* ptr)
{
	int i = 0;

#ifdef TCC_MW_MEMORYLEAKCHECK

	pthread_mutex_lock(&g_TccMw_MemoryLeakCheck_Mutex);

	if(NULL == ptr)
	{
		printf("[%s] free 1 / Critical Error Happened / Please check this log !!!!!\n", __FUNCTION__);
		return ;
	}
	
	for(i = 0; i < TCC_MW_MEMORYLEAKCHECK_STRUCT_MAX; i++)
	{
		if(g_TccMw_MemoryLeakCheck_Struct[i].usage == 1 && g_TccMw_MemoryLeakCheck_Struct[i].ptr == ptr)
		{
			g_TccMw_MemoryLeakCheck_Struct[i].usage = 0;

			g_TccMw_MemoryLeakCheck_CurrentTotalMallocSize -= g_TccMw_MemoryLeakCheck_Struct[i].size;
			g_TccMw_MemoryLeakCheck_CurrentTotalMallocCount--;

			break;
		}
	}

	if(i == TCC_MW_MEMORYLEAKCHECK_STRUCT_MAX)
	{
		printf("[%s] free 2 / Critical Error Happened / Please check this log !!!!! / function %s, line %d\n",
			__FUNCTION__, functionName, line);
	}

	pthread_mutex_unlock(&g_TccMw_MemoryLeakCheck_Mutex);

#endif

	free(ptr);
}

