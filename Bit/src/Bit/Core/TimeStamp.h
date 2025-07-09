#pragma once

#include <chrono>


namespace BitEngine 
{

class Time
{
private:
    float m_LastFrameTime = 0.0f;
    float m_DeltaTime = 0.0f;
    
public:
    void Reset()
    {
        m_LastFrameTime = GetCurrentTime();
    }
    
    void Update()
    {
        float currentTime = GetCurrentTime();
        m_DeltaTime = currentTime - m_LastFrameTime;
        m_LastFrameTime = currentTime;
        
        // Cap delta time to prevent huge jumps
        if (m_DeltaTime > 0.1f)
            m_DeltaTime = 0.1f;
    }
    
    float GetDeltaTime() const { return m_DeltaTime; }
    
private:
    float GetCurrentTime()
    {
        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<float>(currentTime - startTime).count();
    }
};
}

