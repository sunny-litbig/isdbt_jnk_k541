
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "LB_debug_log.h"


#define LB_MAX_LOG_MSG_SIZE 256
static char g_LB_logMsg[ LB_MAX_LOG_MSG_SIZE ] = {0, };
static pthread_mutex_t g_LB_logMutex;
static LB_LOG_TYPE g_LB_logType = LB_LOG_TYPE_ALL_ON;


void LB_Debug_InitLog(void)
{
	pthread_mutex_init(&g_LB_logMutex, 0);
}

void LB_Debug_DeinitLog(void)
{
	pthread_mutex_destroy(&g_LB_logMutex);
}

void LB_Debug_SetLogInfo(unsigned int _logType)
{
	g_LB_logType = _logType;
}

unsigned int LB_Debug_GetLogInfo(void)
{
	return g_LB_logType;
}

void LB_Debug_Message(LB_LOG_LEVEL _logLevel, const char* _format, ...)
{
	pthread_mutex_lock(&g_LB_logMutex);
		
	va_list vaList;

	if( !(LB_LOG_LEVEL_VERBOSE <= _logLevel && _logLevel <= LB_LOG_LEVEL_ERROR) )
	{
		pthread_mutex_unlock(&g_LB_logMutex);
		return;
	}

	switch(g_LB_logType)
	{
		case LB_LOG_TYPE_ALL_ON:
			break;
			
		case LB_LOG_TYPE_IWE_ON:
			if(_logLevel == LB_LOG_LEVEL_VERBOSE || _logLevel == LB_LOG_LEVEL_DEBUG)
			{
				pthread_mutex_unlock(&g_LB_logMutex);
				return;
			}
			break;
			
		case LB_LOG_TYPE_E_ON:
			if( !(_logLevel == LB_LOG_LEVEL_ERROR) )
			{
				pthread_mutex_unlock(&g_LB_logMutex);
				return;
			}
			break;
			
		case LB_LOG_TYPE_ALL_OFF:
		default:
			pthread_mutex_unlock(&g_LB_logMutex);
			return;
	}
	
	va_start(vaList, _format);
	vsnprintf(g_LB_logMsg, LB_MAX_LOG_MSG_SIZE, _format, vaList);

	printf("%s\n", g_LB_logMsg);
	
	va_end(vaList);

	pthread_mutex_unlock(&g_LB_logMutex);
}

