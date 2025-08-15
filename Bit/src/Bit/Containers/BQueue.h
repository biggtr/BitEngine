#pragma once 
#include "Bit/Core/Defines.h"
#include "Bit/Core/Logger.h"
#include <cstring>

namespace BContainer
{


template<typename T>
class Queue
{
private:
    T* m_Data;
    u32 m_ElementCount; // length of the elements
    u32 m_AllocatedSize; // allocated size in bytes

public:
    Queue()
    {
        m_Data = new T[1];
        m_ElementCount = 0;
        m_AllocatedSize = sizeof(T);
    }
    ~Queue()
    {
        delete[] m_Data;
    }

    void PushBack(T value)
    {
        Resize(m_ElementCount + 1);
        m_Data[m_ElementCount] = value;
        m_ElementCount++;
    }
    T Pop()
    {
        if(m_ElementCount == 0)
        {
            BIT_LOG_ERROR("Queue is already Empty");
            return T{};
        }
        T front{};
        memcpy(&front, m_Data, sizeof(T));
        if(m_ElementCount > 1)
            memmove(m_Data, m_Data + 1, sizeof(T) * (m_ElementCount - 1));

        --m_ElementCount;
        return front; 
    }
    T Back()
    {
        if(m_ElementCount == 0)
        {
            BIT_LOG_ERROR("Queue is already Empty");
            return T{};
        }
        return m_Data[--m_ElementCount];
    }
    void Resize(u32 count)
    {
        if(m_AllocatedSize < sizeof(T) * count)
        {
            u32 newCapacity = count;
            T* temp = new T[newCapacity];
            if(m_Data && m_ElementCount > 0)
            {
                memcpy(temp, m_Data, m_ElementCount * sizeof(T));
                delete[] m_Data;
            }
            m_Data = temp;
            m_AllocatedSize = newCapacity * sizeof(T);
        }
    }
    void Clear()
    {
        m_ElementCount = 0; // dont remove the already allocated size to reuse it 
    }
    u32 Size() { return m_ElementCount; }
    b8 IsEmpty() { return m_ElementCount == 0; }
    
    
};

}
