#pragma once

#include "Bitpch.h"
#include <format>

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
    std::string File;
    bool Handled = false;

    LogMessage(LOG_TYPE logType, const std::string& message, size_t line, const std::string& file)
        : LogType(logType), Message(message), Line(line), File(file)   
    {

    }


};

class Logger
{

public:

    static void Init();
    static Logger& GetCoreLogger()
    {
        static Logger instance(LOGGER_TYPE::CORE);
        return instance;
    }
    static Logger& GetClientLogger()
    {

        static Logger instance(LOGGER_TYPE::CLIENT);
        return instance;
    }
    
    void Error(const std::string& msg, size_t line, const std::string& file);
    void Warn(const std::string& msg, size_t line, const std::string& file);
    void Info(const std::string& msg, size_t line, const std::string& file);

    bool HasErrors() const;
    void Clear();
private: 
    Logger(LOGGER_TYPE loggerType, bool useColors = true)
    {
        m_LoggerType = loggerType;
        m_UseColors = useColors;
    }
    ~Logger();
    void Log(LOG_TYPE logType, const std::string& msg, size_t line, const std::string& file);

private:
    void PrintMessage(LogMessage& logMessage);
    bool m_UseColors;
    LOGGER_TYPE m_LoggerType;
};
}

// Core Log Macros
#ifdef _DEBUG
#define BIT_CORE_ERROR(fmt, ...)        BitEngine::Logger::GetCoreLogger().Error(std::format(fmt, ##__VA_ARGS__), __LINE__, __FILE__)
#define BIT_CORE_WARN(fmt, ...)        BitEngine::Logger::GetCoreLogger().Warn(std::format(fmt, ##__VA_ARGS__), __LINE__, __FILE__)
#define BIT_CORE_INFO(fmt, ...)        BitEngine::Logger::GetCoreLogger().Info(std::format(fmt, ##__VA_ARGS__), __LINE__, __FILE__)

// Client Log Macros
#define BIT_CLIENT_ERROR(fmt, ...)        BitEngine::Logger::GetClientLogger().Error(std::format(fmt, ##__VA_ARGS__), __LINE__, __FILE__)
#define BIT_CLIENT_WARN(fmt, ...)        BitEngine::Logger::GetClientLogger().Warn(std::format(fmt, ##__VA_ARGS__), __LINE__, __FILE__)
#define BIT_CLIENT_INFO(fmt, ...)        BitEngine::Logger::GetClientLogger().Info(std::format(fmt, ##__VA_ARGS__), __LINE__, __FILE__)
#else 
#define BIT_CORE_ERROR(fmt, ...)   
#define BIT_CORE_WARN(fmt, ...)    
#define BIT_CORE_INFO(fmt, ...)    
#define BIT_CLIENT_ERROR(fmt, ...) 
#define BIT_CLIENT_WARN(fmt, ...)  
#define BIT_CLIENT_INFO(fmt, ...)  
#endif    

