#include "Bitpch.h"
#include "Logger.h"

namespace BitEngine 
{


void Logger::Error(const std::string& msg, size_t line, const std::string& file)
{
    BitEngine::LogMessage logMessage(LOG_TYPE::ERROR, msg, line, file);
    PrintMessage(logMessage);
}
void Logger::Warn(const std::string& msg, size_t line, const std::string& file)
{
    BitEngine::LogMessage logMessage(LOG_TYPE::WARNING, msg, line, file);
    PrintMessage(logMessage);
}
void Logger::Info(const std::string& msg, size_t line, const std::string& file)
{
    BitEngine::LogMessage logMessage(LOG_TYPE::INFO, msg, line, file);
    PrintMessage(logMessage);
}

// bool Logger::HasErrors() const
// {
//     return std::any_of(m_Messages.begin(),m_Messages.end(),
//             [](const auto& msg) { return msg.LogType == LOG_TYPE::ERROR; }
//             );
// }
void Logger::PrintMessage(LogMessage& msg) 
{
        if(!msg.Handled)
        {
            std::cerr << "["
                << (msg.LoggerType == LOGGER_TYPE::CLIENT ? "CLIENT:" : "CORE:")
                << (msg.LogType == LOG_TYPE::ERROR ? "ERROR" : 
                msg.LogType == LOG_TYPE::WARNING ? "WARN" : "INFO")
                << "] Line " << msg.Line << ", Col " << msg.File
                << ": " << msg.Message << "\n";
        }
        msg.Handled = true;

}
Logger::~Logger()
{
}

}
