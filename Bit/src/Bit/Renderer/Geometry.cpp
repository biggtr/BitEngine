#pragma once 
#include "Geometry.h"

namespace BitEngine
{
Geometry::Geometry(const std::string& name)
{
    m_Name = name;
}
Geometry::~Geometry()
{
    delete m_VBO;
    delete m_IBO;
    delete m_VAO;
    delete m_Material;
}

void Geometry::SetVertices(const std::vector<Vertex3D>& vertices)
{
    m_Vertices = vertices;
    MakeDirty();
}
void Geometry::SetIndices(const std::vector<u32> indices)
{
    m_Indices = indices;
    MakeDirty();
}
void Geometry::UploadVertices(const std::vector<Vertex3D>& m_Vertices) // dynamic meshes
{

}

void Geometry::AddVertex(const Vertex3D& vertex);
void Geometry::AddTriangle(u32 a, u32 b, u32 c);
void Geometry::AddQuad(u32 a, u32 b, u32 c, u32 d);

void Geometry::CalculateNormals();
void Geometry::CalculateTangents();
void Geometry::CalculateBounds();
void Geometry::FlipNormals();
void Geometry::Center();   // center the mesh around the origin


void Geometry::SetMaterial(Material* material);
Material* Geometry::GetMaterial();

void Geometry::Translate(const BMath::Vec3& translation);
void Geometry::Rotate(f32 angle, const BMath::Vec3& axis);
void Geometry::SetScale(f32 scale);
void Geometry::SetScale(const BMath::Vec3& scale);

bool Geometry::IsUploaded() const { return m_IsUploaded; }
bool Geometry::IsDynamic() const { return m_IsDynamic; }

const std::string& Geometry::GetName() const { return m_Name; }
const std::vector<Vertex3D>& Geometry::GetVertices() const { return m_Vertices; }
const std::vector<u32>& Geometry::GetIndices() const { return m_Indices; }
VertexArray* Geometry::GetVertexArray() const { return m_VAO; }

u32 Geometry::GetVertexCount() const { return static_cast<u32>(m_Vertices.size()); }
u32 Geometry::GetIndexCount() const { return static_cast<u32>(m_Indices.size()); }
u32 Geometry::GetTriangleCount() const { return GetIndexCount() / 3; }

void Geometry::CreateBuffers();
void Geometry::UpdateBuffers();
BufferLayout Geometry::CreateVertexLayout() const;
};
}
