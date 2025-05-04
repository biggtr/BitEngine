#pragma once

#include <chrono>


namespace BitEngine 
{

class Time 
{
public:
    Time()
        : m_StartTime(Clock::now()), m_LastTime(m_StartTime) // initialize lasttime with starttime to avoid garbage value at first
    {
    }

    double GetTotalTime()
    {
        auto now = Clock::now();
        return std::chrono::duration<double>(now - m_StartTime).count();
    }

    float GetDeltaTime()
    {
        auto currentTime = Clock::now();
        auto delta = currentTime - m_LastTime;
        m_LastTime = currentTime;
        return std::chrono::duration<float>(delta).count();
    }

private:
    using Clock = std::chrono::steady_clock;
    Clock::time_point m_StartTime;
    Clock::time_point m_LastTime;

};

}

