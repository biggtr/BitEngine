#pragma once

#include "Bit/Core/Defines.h"
#include <chrono>


namespace BitEngine 
{

class Time
{
private:
    f64 m_LastFrameTime = 0.0f;
    f64 m_DeltaTime = 0.0f;
    
public:
    void Reset()
    {
        m_LastFrameTime = GetCurrentTime();
    }
    
    void Update()
    {
        f64 currentTime = GetCurrentTime();
        m_DeltaTime = currentTime - m_LastFrameTime;
        m_LastFrameTime = currentTime;
        
        // Cap delta time to prevent huge jumps
        if (m_DeltaTime > 0.1f)
            m_DeltaTime = 0.1f;
    }
    
    f64 GetDeltaTime() const { return m_DeltaTime; }
    
private:
    f64 GetCurrentTime()
    {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<f64>(currentTime - startTime).count();
    }
};
}

