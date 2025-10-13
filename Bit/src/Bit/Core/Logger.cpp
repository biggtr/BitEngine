#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include "Logger.h"
#include "Logger.h"
#include <cstdio>
#include <stdarg.h>
#include "assert.h"

struct LoggerState
{
    u64 MaxLoggerFilesSize;
};
static LoggerState* loggerState = 0;
b8 LoggerInitialize(u64* memoryRequirement, void* state)
{
    *memoryRequirement = sizeof(LoggerState);
    if(!state)
    {
        return true;
    }
    loggerState = (LoggerState*)state;
    memset(loggerState, 0, sizeof(LoggerState));
    return true;
}
void LoggerShutdown(void* state)
{
    if(state)
    {
        loggerState = 0;
    }
    BIT_LOG_INFO("Event System Is shutting down..!");
}


void Log(LOG_LEVEL level, const char* message, ...)
{
    const char* logLevel[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
    char outMessage[64000];
    va_list args;
    va_start(args, message);
    vsnprintf(outMessage, sizeof(outMessage), message, args);
    va_end(args);

    char finalOutMessage[64000];
    sprintf(finalOutMessage, "%s%s\n", logLevel[(u8)level], outMessage);

    printf("%s", finalOutMessage);
}
b8 ShutdownLogging()
{
    return TRUE;
}
void ReportAssertionFailure(const char* expression, const char* message, const char* fileName, u32 line)
{
    Log(LOG_LEVEL_FATAL,
        "Assertion Failure: '%s', message: '%s', in file '%s', line : %d",
        expression, message,
        fileName, line
       );
}
