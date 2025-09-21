#include "GeometryManager.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Geometry.h"
#include <vector>

namespace BitEngine
{


GeometryManager::GeometryManager()
{

}
GeometryManager::~GeometryManager()
{
    Clear();
}

Geometry* GeometryManager::CreateGeometry(const std::string& name)
{
    if(HasGeometry(name))
    {
        BIT_LOG_WARN("Geometry %s is already there ", name.c_str());
        return GetGeometry(name);
    }
    Geometry* geometry = new Geometry(name);
    m_Geometries[name] = geometry;
    return m_Geometries.at(name);
}
Geometry* GeometryManager::LoadGeometry(const std::string& name, std::string filePath)
{
    if(HasGeometry(name))
    {
        BIT_LOG_WARN("Geometry %s is already there ", name.c_str());
        return GetGeometry(name);
    }
    Geometry* geometry = new Geometry(name);
    if(LoadFromFile(geometry, filePath))
    {
        delete geometry;        
        BIT_LOG_ERROR("Failed to load geometry from %s", filePath.c_str());
        return nullptr;
    }

    m_Geometries[name] = geometry;
    return m_Geometries.at(name);
}

Geometry* GeometryManager::CreateCube(const std::string& name, f32 size)
{
    if(HasGeometry(name))
    {
        return m_Geometries.at(name);
    }
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    GenerateCubeData(vertices, indices, size);
    Geometry* geometry = new Geometry(name);
    geometry->SetVertices(vertices);
    geometry->SetIndices(indices);
    geometry->UploadToGPU();
    m_Geometries[name] = geometry;
    return m_Geometries.at(name);
}
Geometry* GeometryManager::CreateSphere(const std::string& name, f32 radius, u32 segments)
{
    if(HasGeometry(name))
    {
        return m_Geometries.at(name);
    }
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    GenerateSphereData(vertices, indices, radius, segments);
    Geometry* geometry = new Geometry(name);
    geometry->SetVertices(vertices);
    geometry->SetIndices(indices);
    geometry->UploadToGPU();
    m_Geometries[name] = geometry;
    return m_Geometries.at(name);
}
Geometry* GeometryManager::CreatePlane(const std::string& name, f32 width, f32 height)
{

    if(HasGeometry(name))
    {
        return m_Geometries.at(name);
    }
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    GeneratePlaneData(vertices, indices, width, height);
    Geometry* geometry = new Geometry(name);
    geometry->SetVertices(vertices);
    geometry->SetIndices(indices);
    geometry->UploadToGPU();
    m_Geometries[name] = geometry;
    return m_Geometries.at(name);
}

Geometry* GeometryManager::GetGeometry(const std::string& name)
{
    if(HasGeometry(name))
    {
        return m_Geometries.at(name);
    }
    return CreateCube("DefaultCube");
}
b8 GeometryManager::HasGeometry(const std::string& name) const
{
    return m_Geometries.contains(name);
}
b8 GeometryManager::RemoveGeometry(const std::string& name)
{
    if(!HasGeometry(name))
    {
        BIT_LOG_WARN("Geometry %s, is not there already");
        return false;
    }
    delete m_Geometries.at(name);
    return m_Geometries.erase(name);
}
void GeometryManager::Clear()
{
    for(auto& geometry : m_Geometries)
    {
        delete geometry.second;
    }
}

std::vector<std::string> GeometryManager::GetGeometryNames() const
{
    std::vector<std::string> geometryNames;
    for(auto& geometry : m_Geometries)
    {
        geometryNames.push_back(geometry.first);
    }
    return geometryNames;
}
u32 GeometryManager::GetTotalVertices() const
{
    u32 total = 0;
    for(auto& geometry : m_Geometries)
    {
        total += geometry.second->GetVertexCount();
    }
    return total;
}
u32 GeometryManager::GetTotalTriangles() const
{
    u32 total = 0;
    for(auto& geometry : m_Geometries)
    {
        total += geometry.second->GetTriangleCount();
    }
    return total;
}

void GeometryManager::LoadBuiltinPrimitives()
{
    CreateCube("cube", 1.0f);
    CreateSphere("sphere", 1.0f, 32);
    CreatePlane("plane", 1.0f, 1.0f);
}

void GeometryManager::GenerateCubeData(std::vector<Vertex>& vertices, std::vector<u32>& indices, f32 size);
void GeometryManager::GenerateSphereData(std::vector<Vertex>& vertices, std::vector<u32>& indices, f32 radius, u32 segments);
void GeometryManager::GeneratePlaneData(std::vector<Vertex>& vertices, std::vector<u32>& indices, f32 width, f32 height);

}
