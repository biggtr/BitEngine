#pragma once 

#include "Bit/Core/Defines.h"
#include "Bit/Math/Transform.h"
#include <string>
#include <vector>
namespace BitEngine
{
class Geometry;
class Mesh
{
private:
    std::vector<Geometry*> m_Geometries;
    BMath::Transform Transform;
    std::string m_Name;
public:
    Mesh();
    ~Mesh();

    void AddGeometry(Geometry* geometry);
    u32 GetGeometryCount();

    void SetName(const std::string& name);
    std::string GetName();

    
};
}
