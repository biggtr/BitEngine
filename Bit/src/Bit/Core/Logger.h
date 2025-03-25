#include <iostream>
#include <string>
#include <vector>

enum class LogType
{
    WARNING,
    INFO,
    ERROR
};

struct LogMessage
{
    LogType LogType;
    std::string Message;
    size_t Line;
    size_t Column;
};

class Logger
{

public:

    static Logger& GetInstance(bool useColors = true);
    
    static void Error(const std::string& msg, size_t line, size_t column);
    static void Warning(const std::string& msg, size_t line, size_t column);
    static void Info(const std::string& msg, size_t line, size_t column);

    bool HasErrors() const;
    void PrintMessages() const;
    void Clear();
private: 
    Logger(bool useColors)
    {
        m_UseColors = useColors;
    }
    void Log(LogType logType, const std::string msg, size_t line, size_t column);

private:
    bool m_UseColors;
    std::vector<LogMessage> m_Messages;
};
