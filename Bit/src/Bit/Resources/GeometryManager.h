#pragma once

#include "Bit/Renderer/Geometry.h"
#include "Bit/Renderer/Mesh.h"
#include <string>
#include <unordered_map>
namespace BitEngine
{

class GeometryManager
{
private:
    std::unordered_map<std::string, Geometry*> m_Geometries;
    
    //temp
    MaterialManager* m_MaterialManager;
    TextureManager* m_TextureManager;
public:
    GeometryManager(MaterialManager* materialManager)
    {
        m_MaterialManager = materialManager;
    }
    ~GeometryManager();

    Geometry* CreateGeometry(const std::string& name);
    Mesh* LoadFromFile(std::string meshName,  std::string filepath);

    Geometry* CreateCube(const std::string& name, f32 size, const BMath::Vec4& color = { 1.0f, 0.0f, 0.0f, 1.0f});
    Geometry* CreateSphere(const std::string& name, f32 radius = 1.5f, u32 segments = 32);
    Geometry* CreatePlane(const std::string& name, f32 width = 1.0f, f32 height = 1.0f, const BMath::Vec4& color = { 0.0f, 1.0f, 0.0f, 1.0f});
    Geometry* CreateCylinder(const std::string& name, f32 height = 3.0f, u32 segments = 32);
    Geometry* CreateQuad(const std::string& name, f32 size = 2.0f, const BMath::Vec4& color = {1.0f, 1.0f, 1.0f,1.0f});
    Geometry* GetGeometry(const std::string& name);
    bool HasGeometry(const std::string& name) const;
    bool RemoveGeometry(const std::string& name);
    void Clear();

    std::vector<std::string> GetGeometryNames() const;
    u32 GetGeometryCount() const { return (u32)(m_Geometries.size()); }
    u32 GetTotalVertices() const;
    u32 GetTotalTriangles() const;

    void LoadBuiltinPrimitives();
    
private:
    void GenerateCubeData(std::vector<Vertex>& vertices, std::vector<u32>& indices, f32 size, const BMath::Vec4& color);
    void GenerateSphereData(std::vector<Vertex>& vertices, std::vector<u32>& indices, f32 radius, u32 segments);
    void GeneratePlaneData(std::vector<Vertex>& vertices, std::vector<u32>& indices, f32 width, f32 height, const BMath::Vec4& color);
    void GenerateQuadData(std::vector<Vertex>& vertices, std::vector<u32>& indices, f32 size, const BMath::Vec4& color);
};

}
