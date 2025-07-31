#pragma once

#include "Bitpch.h"

namespace BitEngine 
{

enum class LOG_LEVEL : uint8_t
{
    FATAL = 0,
    ERROR = 1,
    WARN = 2,
    INFO = 3,
    DEBUG = 4,
    TRACE = 5
};

class Logger
{
private:
    static Logger* s_Instance;
public:

    static bool Initialize();

    static void Log(LOG_LEVEL level, const char* msg, ...);

    static bool Shutdown();

    static const char* GetLevelString(LOG_LEVEL level);
private:
    void LogInternal(LOG_LEVEL level, const char* msg);

};


#define BIT_LOG_FATAL(message, ...) BitEngine::Logger::Log(BitEngine::LOG_LEVEL::FATAL, message, ##__VA_ARGS__);
#define BIT_LOG_ERROR(message, ...) BitEngine::Logger::Log(BitEngine::LOG_LEVEL::ERROR, message, ##__VA_ARGS__);
#define BIT_LOG_TRACE(message, ...) BitEngine::Logger::Log(BitEngine::LOG_LEVEL::TRACE, message, ##__VA_ARGS__);
#define BIT_LOG_DEBUG(message, ...) BitEngine::Logger::Log(BitEngine::LOG_LEVEL::DEBUG, message, ##__VA_ARGS__);
#define BIT_LOG_WARN(message, ...)  BitEngine::Logger::Log(BitEngine::LOG_LEVEL::WARN, message, ##__VA_ARGS__);
#define BIT_LOG_INFO(message, ...)  BitEngine::Logger::Log(BitEngine::LOG_LEVEL::INFO, message, ##__VA_ARGS__);
}
