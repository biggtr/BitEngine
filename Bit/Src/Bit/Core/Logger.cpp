#include "Bitpch.h"
#include "Logger.h"

namespace BitEngine 
{

Logger* Logger::s_CoreLogger = nullptr;
Logger* Logger::s_ClientLogger = nullptr;
void Logger::Init()
{
    s_CoreLogger = new Logger(LOGGER_TYPE::CORE, true);
    s_ClientLogger= new Logger(LOGGER_TYPE::CLIENT, true);
}
Logger& Logger::GetCoreLogger()
{
    return *s_CoreLogger;
}
Logger& Logger::GetClientLogger()
{
    return *s_ClientLogger;
}
void Logger::Log(LOG_TYPE logType, const std::string msg, size_t line, size_t column)
{
    m_Messages.push_back({m_LoggerType, logType, msg, line, column});
}

void Logger::Error(const std::string& msg, size_t line, size_t column)
{
    Log(LOG_TYPE::ERROR, msg, line, column);
}
void Logger::Warning(const std::string& msg, size_t line, size_t column)
{
    Log(LOG_TYPE::WARNING, msg, line, column);
}
void Logger::Info(const std::string& msg, size_t line, size_t column)
{
    Log(LOG_TYPE::INFO, msg, line, column);
}

bool Logger::HasErrors() const
{
    return std::any_of(m_Messages.begin(),m_Messages.end(),
            [](const auto& msg) { return msg.LogType == LOG_TYPE::ERROR; }
            );
}
void Logger::PrintMessages() 
{
    for(auto& msg : m_Messages)
    {
        if(!msg.Handled)
        {
            std::cerr << "["
                << (msg.LoggerType == LOGGER_TYPE::CLIENT ? "CLIENT:" : "CORE:")
                << (msg.LogType == LOG_TYPE::ERROR ? "ERROR" : 
                msg.LogType == LOG_TYPE::WARNING ? "WARN" : "INFO")
                << "] Line " << msg.Line << ", Col " << msg.Column
                << ": " << msg.Message << "\n";
        }
        msg.Handled = true;
    }

}
void Logger::Clear()
{
    m_Messages.clear();
}
Logger::~Logger()
{
    delete s_CoreLogger;
    delete s_ClientLogger;
}

}
