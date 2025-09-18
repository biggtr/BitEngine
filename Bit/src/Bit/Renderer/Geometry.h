#pragma once 
#include "Bit/Core/Defines.h"
#include "Bit/Renderer/Buffers.h"
#include "Bit/Renderer/Material.h"
#include "Bit/Renderer/VertexArray.h"
#include "Bit/Resources/ResourceTypes.h"
#include <vector>

namespace BitEngine
{
struct Vertex3D
{
    BMath::Vec3 Position;
    BMath::Vec3 Normal;
    BMath::Vec2 TextureCoords;
    BMath::Vec4 Color;
};

#define MAX_GEOMETRY_NAME_LENGTH 256
class Geometry
{
private:
    std::string m_Name;
    std::vector<Vertex3D> m_Vertices;
    std::vector<u32> m_Indices;
    VertexArray*  m_VAO;
    VertexBuffer* m_VBO;
    IndexBuffer*  m_IBO;
    Material* m_Material;
    BMath::Mat4 m_Transform;
    bool m_IsUploaded;
    bool m_IsDynamic;
public:
    Geometry(const std::string& name);
    ~Geometry();

    void SetVertices(const std::vector<Vertex3D>& m_Vertices);
    void SetIndices(const std::vector<u32> indices);
    void UploadVertices(const std::vector<Vertex3D>& m_Vertices); // dynamic meshes

    void AddVertex(const Vertex3D& vertex);
    void AddTriangle(u32 a, u32 b, u32 c);
    void AddQuad(u32 a, u32 b, u32 c, u32 d);

    void CalculateNormals();
    void CalculateTangents();
    void CalculateBounds();
    void FlipNormals();
    void Center();   // center the mesh around the origin


    void SetMaterial(Material* material);
    Material* GetMaterial();

    void Translate(const BMath::Vec3& translation);
    void Rotate(f32 angle, const BMath::Vec3& axis);
    void SetScale(f32 scale);
    void SetScale(const BMath::Vec3& scale);

    bool IsUploaded() const { return m_IsUploaded; }
    bool IsDynamic() const { return m_IsDynamic; }

    const std::string& GetName() const { return m_Name; }
    const std::vector<Vertex3D>& GetVertices() const { return m_Vertices; }
    const std::vector<u32>& GetIndices() const { return m_Indices; }
    VertexArray* GetVertexArray() const { return m_VAO; }
    
    u32 GetVertexCount() const { return static_cast<u32>(m_Vertices.size()); }
    u32 GetIndexCount() const { return static_cast<u32>(m_Indices.size()); }
    u32 GetTriangleCount() const { return GetIndexCount() / 3; }

private:
    void CreateBuffers();
    void UpdateBuffers();
    BufferLayout CreateVertexLayout() const;
};
}
