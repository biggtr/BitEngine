#include "Geometry.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Buffers.h"

namespace BitEngine {
using BMath::Mat4;

Geometry::Geometry(const std::string& name)
    : m_VAO(), m_VBO(), m_IBO(), 
        m_Material(), m_Transform(), m_IsUploaded(false), m_IsDynamic(false)
{
    m_Name = name;
}
Geometry::~Geometry()
{
    delete m_IBO;
    delete m_VAO;
    delete m_Material;
}

void Geometry::SetVertices(const std::vector<Vertex>& vertices)
{
    m_Vertices = vertices;
    MakeDirty();
}
void Geometry::SetIndices(const std::vector<u32> indices)
{
    m_Indices = indices;
    MakeDirty();
}
void Geometry::UploadVertices(const std::vector<Vertex>& m_Vertices) // dynamic meshes
{

}

void Geometry::AddVertex(const Vertex& vertex)
{
    m_Vertices.push_back(vertex);
    MakeDirty();
}
void Geometry::AddTriangle(u32 a, u32 b, u32 c)
{
    m_Indices.push_back(a);
    m_Indices.push_back(b);
    m_Indices.push_back(b);
}
void Geometry::AddQuad(u32 a, u32 b, u32 c, u32 d)
{
    AddTriangle(a, b, c);
    AddTriangle(a, c, d);
}

void Geometry::CalculateNormals()
{

}
void Geometry::CalculateTangents()
{

}
void Geometry::CalculateBounds()
{

}
void Geometry::FlipNormals()
{

}
void Geometry::Center()   // center the mesh around the origin
{

}


void Geometry::SetMaterial(Material* material)
{
    m_Material = material;
}
Material* Geometry::GetMaterial()
{
    if(!m_Material)
    {
        BIT_LOG_ERROR("MATERIAL was not found for this Geometry %s", m_Name.c_str());
        return 0;
    }
    return m_Material;
}

void Geometry::Translate(const BMath::Vec3& translation)
{
  Mat4 t = BMath::Mat4Scale(translation.x, translation.y, translation.z);
  m_Transform = m_Transform * t;
}
void Geometry::Rotate(f32 angle, const BMath::Vec3& axis)
{
    BMath::Vec3 scaledAxis = axis * angle;
    Mat4 r = BMath::Mat4Rotate(scaledAxis.x, scaledAxis.y, scaledAxis.z);
    m_Transform = m_Transform * r;
}
void Geometry::Scale(const BMath::Vec3& scale)
{
    Mat4 s = BMath::Mat4Scale(scale.x, scale.y, scale.z);
    m_Transform = m_Transform * s;
}
void Geometry::Scale(f32 scale)
{
    Scale({scale, scale, scale});
}

b8 Geometry::UploadToGPU()
{
    if(m_IsUploaded && !m_IsDynamic)
    {
        return true;
    }
    if(m_Vertices.empty() || m_Indices.empty())
    {
        BIT_LOG_ERROR("No Vertices or indices to upload to gpu");
        return false;
    }
    CreateBuffers();
    if(m_IsDynamic)
    {
        UpdateBuffers();
    }
    m_IsUploaded = true;
    return true;
}
void Geometry::CreateBuffers()
{

    m_VAO = VertexArray::Create();

    u32 vbosize = sizeof(Vertex) * m_Vertices.size();


    if(m_IsDynamic)
    {
        m_VBO = VertexBuffer::Create(vbosize);
        m_VBO->SetData(m_Vertices.data(), vbosize);

        u32 indexCount = (u32)m_Indices.size();
        m_IBO = IndexBuffer::Create(indexCount);  
        m_IBO->SetData(m_Indices.data(), indexCount);  
    }
    else
    {
        f32* vertexData = (f32*)m_Vertices.data();
        m_VBO = VertexBuffer::Create(vertexData, vbosize);
        m_IBO = IndexBuffer::Create(m_Indices.data(), m_Indices.size());
    }

    m_VBO->SetBufferLayout(CreateVertexLayout());
    m_VAO->AddVertexBuffer(m_VBO);
    m_VAO->SetIndexBuffer(m_IBO);
}
void Geometry::UpdateBuffers()
{
    if(!m_VBO || !m_IBO)
        return;

    u32 vbosize = sizeof(Vertex) * m_Vertices.size();
    m_VBO->SetData(m_Vertices.data(), vbosize);

    m_IBO->SetData(m_Indices.data(), m_Indices.size());
}
BufferLayout Geometry::CreateVertexLayout() const
{
    return BufferLayout({
        { SHADER_DATA_TYPE::FLOAT4, "a_Position"}, 
        { SHADER_DATA_TYPE::FLOAT4, "a_Color"}, 
        { SHADER_DATA_TYPE::FLOAT2, "a_TexCoords"}, 
        });
}
} 
