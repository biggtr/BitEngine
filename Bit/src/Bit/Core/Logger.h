#pragma once 

#include "Defines.h"

typedef enum 
{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,
} LOG_LEVEL;

b8 LoggerInitialize(u64* memoryRequirement, void* state);
void LoggerShutdown(void* state);
void Log(LOG_LEVEL, const char* message, ...);




#define BIT_LOG_FATAL(message, ...) Log(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);
#define BIT_LOG_ERROR(message, ...) Log(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#define BIT_LOG_TRACE(message, ...) Log(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#define BIT_LOG_DEBUG(message, ...) Log(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#define BIT_LOG_WARN(message, ...)  Log(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#define BIT_LOG_INFO(message, ...)  Log(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
