#include "Bitpch.h"
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include "Logger.h"

namespace BitEngine 
{
Logger* Logger::s_Instance = nullptr; 

bool Logger::Initialize()
{
    if(s_Instance)
    {
        printf("You Cannot Create More Than One Logger Instance At A Time\n");
        return false;
    }
    s_Instance = new Logger();
    return true;
}

void Logger::Log(LOG_LEVEL level, const char* msg, ...)
{

    constexpr size_t STACK_BUFFER_SIZE = 2048;
    char stackBuffer[STACK_BUFFER_SIZE];
    
    va_list args;
    va_start(args, msg);

    int result = vsnprintf(stackBuffer, STACK_BUFFER_SIZE, msg, args); 
    va_end(args);

                          
    if(result < STACK_BUFFER_SIZE)
    {
        s_Instance->LogInternal(level, stackBuffer);
    }
    else 
    {
        int size = result + 1; // +1 for nul term
        char* buffer = new char[size]; 

        va_start(args, msg);
        vsnprintf(buffer, size, msg, args);
        va_end(args);
        s_Instance->LogInternal(level, buffer);
        delete[] buffer;
    }
}                         
void Logger::LogInternal(LOG_LEVEL level, const char* msg)
{
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    printf("[%04d-%02d-%02d %02d:%02d:%02d] [%-5s] %s\n",
           timeinfo->tm_year + 1900,
           timeinfo->tm_mon + 1,
           timeinfo->tm_mday,
           timeinfo->tm_hour,
           timeinfo->tm_min,
           timeinfo->tm_sec,
           GetLevelString(level),
           msg);
}
                          
const char* Logger::GetLevelString(LOG_LEVEL level)
{
    const char* levelStrings[] = {"FATAL" , "ERROR", "WARN", "INFO", "DEBUG", "TRACE" };
    uint8_t index = (uint8_t)level;
    return (index > 0 && index < 6) ? levelStrings[index] : "UNKOWN";
}
bool Logger::Shutdown()   
{                         
    if(!s_Instance)
        return false;
    delete s_Instance;
    s_Instance = nullptr;
    return true;
}

}
