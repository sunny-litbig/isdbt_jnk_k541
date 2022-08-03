
#ifndef LB_DEBUG_LOG_H
#define LB_DEBUG_LOG_H

#ifdef __cplusplus
extern "C" {
#endif


typedef enum _LB_LOG_LEVEL
{
	LB_LOG_LEVEL_VERBOSE = 1,
	LB_LOG_LEVEL_DEBUG = 2,
	LB_LOG_LEVEL_INFO = 4,
	LB_LOG_LEVEL_WARN = 8,
	LB_LOG_LEVEL_ERROR = 16,
}LB_LOG_LEVEL;

typedef enum _LB_LOG_TYPE
{
	LB_LOG_TYPE_ALL_ON,    // Turn on all log
	LB_LOG_TYPE_IWE_ON,    // Turn on info, warn, error logs
	LB_LOG_TYPE_E_ON,      // Turn on error log
	LB_LOG_TYPE_ALL_OFF    // Turn off all log
}LB_LOG_TYPE;


void LB_Debug_InitLog(void);

void LB_Debug_DeinitLog(void);

extern void LB_Debug_SetLogInfo(unsigned int _logType);

extern unsigned int LB_Debug_GetLogInfo(void);

extern void LB_Debug_Message(LB_LOG_LEVEL _logLevel, const char* _format, ...);


#ifdef __cplusplus
}
#endif

#endif

