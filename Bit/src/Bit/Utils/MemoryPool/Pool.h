#pragma once


#include <cstdint>
namespace BitEngine
{

class IPool
{
public:
    virtual ~IPool(){}
};

template<typename T>
class Pool : public IPool
{
private:
    std::vector<T> m_Data;
public:
    Pool(uint32_t size){ Resize(size); }
    ~Pool() = default;

    bool IsEmpty() const { return m_Data.empty(); }

    size_t GetSize() const { return m_Data.size(); }
    void Resize(uint32_t n) { m_Data.resize(n); }
    void Clear() { m_Data.clear(); }
    void Add(T object) { m_Data.push_back(object); }
    void Set(uint32_t index, const T& object) { m_Data[index] = object; }
    
    T& Get(uint32_t index) { return m_Data[index]; }

    T& operator [](uint32_t index) { return m_Data[index]; }

};

}
