#pragma once
#include "Bit/Core/Defines.h"
#include <cstdint>
#include <strings.h>
#include <vector>

#define MAX_NUM_ENTITIES 1000000

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
    std::vector<T> m_DenseArray;
    std::vector<u32> m_DenseEntities;
    u32 m_SparseArray[MAX_NUM_ENTITIES];
public:
    Pool(u32 size){ Resize(size); }
    ~Pool() = default;

    bool IsEmpty() const { return m_DenseArray.empty(); }

    size_t GetSize() const { return m_DenseArray.size(); }
    void Resize(u32 n) { m_DenseArray.resize(n); }
    void Clear() { m_DenseArray.clear(); m_DenseEntities.clear(); }
    void Add(T object) { m_DenseArray.push_back(object); }
    void Set(u32 entity, const T& object) 
    {
        u32 denseIndex = m_DenseArray.size();
        m_SparseArray[entity] = denseIndex;
        m_DenseArray.push_back(object);
        m_DenseEntities.push_back(entity);
    }
    void Remove(u32 entity)
    {
        u32 denseIndex = m_SparseArray[entity];

        if(denseIndex < m_DenseArray.size() - 1)
        {
            //removing the element at denseIndex
            m_DenseArray[denseIndex] = m_DenseArray.back();
            m_DenseEntities[denseIndex] = m_DenseEntities.back();

            u32 movedEntity = m_DenseEntities[denseIndex];
            m_SparseArray[movedEntity] = denseIndex;
        }

        m_DenseArray.pop_back();
        m_DenseEntities.pop_back();

        m_SparseArray[entity] = -1;
    }
    
    T& Get(u32 index) 
    { 
        u32 denseIndex = m_SparseArray[index];
        return m_DenseArray[denseIndex]; 
    }

    T& operator [](u32 index) 
    { 
        u32 denseIndex = m_SparseArray[index];
        return m_DenseArray[denseIndex]; 
    }

};

}
