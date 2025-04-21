#include "Logger.h"
#include "Bitpch.h"

namespace BitEngine 
{

    void Logger::Log(LOG_TYPE logType, const std::string msg, size_t line, size_t column)
    {
        m_Messages.push_back({logType, msg, line, column});
    }
    Logger& Logger::GetInstance(bool useColors)
    {
        static Logger instance(useColors);
        return instance;
    }

    void Logger::Error(const std::string& msg, size_t line, size_t column)
    {
        GetInstance().Log(LOG_TYPE::ERROR, msg, line, column);
    }
    void Logger::Warning(const std::string& msg, size_t line, size_t column)
    {
        GetInstance().Log(LOG_TYPE::WARNING, msg, line, column);
    }
    void Logger::Info(const std::string& msg, size_t line, size_t column)
    {
        GetInstance().Log(LOG_TYPE::INFO, msg, line, column);
    }

    bool Logger::HasErrors() const
    {
        return std::any_of(m_Messages.begin(),m_Messages.end(),
                [](const auto& msg) { return msg.LogType == LOG_TYPE::ERROR; }
                );
    }
    void Logger::PrintMessages() const
    {
        for(auto& msg : m_Messages)
        {
            std::cerr << "["
                      << (msg.LogType == LOG_TYPE::ERROR ? "ERROR" : 
                          msg.LogType == LOG_TYPE::WARNING ? "WARN" : "INFO")
                      << "] Line " << msg.Line << ", Col " << msg.Column
                      << ": " << msg.Message << "\n";
        }

    }
    void Logger::Clear()
    {
        m_Messages.clear();
    }
}
