#pragma once
#include "Bit/Core/Defines.h"
#include "Bit/Core/Logger.h"
#include "Bit/Math/BMath.h"
#include <ctime>
namespace BitEngine
{
class ProfilerTime
{
private:
    const char* m_Name;
    clock_t m_StartTime;
    b8 m_Stopped;

public:
    ProfilerTime(const char* name)
        : m_Name(name), m_Stopped(false)
    {
        m_StartTime = clock();
    }
    ~ProfilerTime()
    {
        if(!m_Stopped)
        {
            Stop();
        }
    }
    void Stop()
    {
        clock_t end = clock();
        f64 duration = (end - m_StartTime) / (f64)CLOCKS_PER_SEC;
        BIT_LOG_DEBUG("Time Spent on function : %s %.2f ms", m_Name, duration * B_SEC_TO_MS_MULTIPLIER);
        m_Stopped = true;
    }

};
}

