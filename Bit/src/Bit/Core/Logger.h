#pragma once

#include "Bitpch.h"

namespace BitEngine 
{

enum class LOG_TYPE
{
    WARNING,
    INFO,
    ERROR
};

enum class LOGGER_TYPE
{
    CLIENT, CORE 
};

struct LogMessage
{
    LOGGER_TYPE LoggerType;
    LOG_TYPE LogType;
    std::string Message;
    size_t Line;
    size_t Column;
    bool Handled = false;

    LogMessage(LOG_TYPE logType, const std::string& message, size_t line, size_t column)
        : LogType(logType), Message(message), Line(line), Column(column)
    {

    }
};

class Logger
{

public:

    static void Init();
    static Logger& GetCoreLogger(); 
    static Logger& GetClientLogger(); 
    
    void Error(const std::string& msg, size_t line, size_t column);
    void Warning(const std::string& msg, size_t line, size_t column);
    void Info(const std::string& msg, size_t line, size_t column);

    bool HasErrors() const;
    void PrintMessage(LogMessage logMessage);
    void Clear();
private: 
    Logger(LOGGER_TYPE loggerType, bool useColors)
    {
        m_LoggerType = loggerType;
        m_UseColors = useColors;
    }
    ~Logger();
    void Log(LOG_TYPE logType, const std::string msg, size_t line, size_t column);

private:
    static Logger* s_CoreLogger;
    static Logger* s_ClientLogger;
    bool m_UseColors;
    LOGGER_TYPE m_LoggerType;
};
}


// Core Log Macros
#define BIT_CORE_ERROR(...)     BitEngine::Logger::GetCoreLogger().Error(__VA_ARGS__); 
#define BIT_CORE_WARN(...)      BitEngine::Logger::GetCoreLogger().Warning(__VA_ARGS__); 
#define BIT_CORE_INFO(...)      BitEngine::Logger::GetCoreLogger().Info(__VA_ARGS__); 


// Client Log Macros
#define BIT_CLIENT_ERROR(...)     BitEngine::Logger::GetClientLogger().Error(__VA_ARGS__)
#define BIT_CLIENT_WARN(...)      BitEngine::Logger::GetClientLogger().Warning(__VA_ARGS__)
#define BIT_CLIENT_INFO(...)      BitEngine::Logger::GetClientLogger().Info(__VA_ARGS__)
