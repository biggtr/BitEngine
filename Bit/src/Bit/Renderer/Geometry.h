#pragma once 
#include "Bit/Core/Defines.h"
#include "Bit/Renderer/VertexArray.h"
#include "Bit/Resources/ResourceTypes.h"

namespace BitEngine
{
struct Vertex3D
{
    BMath::Vec3 Position;
    BMath::Vec4 Color;
    BMath::Vec2 TextureCoords;
};

#define MAX_GEOMETRY_NAME_LENGTH 256
class Geometry
{
private:
    VertexArray* VAO;
    char Name[MAX_GEOMETRY_NAME_LENGTH];
    u32 m_VertexCount;
    u32 m_IndexCount;
    Material* Mat;
public:
    Geometry(u32 vertexCount, const Vertex3D* vertices, u32 indexCount, const u32* indices);
    ~Geometry(){}

    VertexArray* GetVAO();
    void SetMaterial(Material* material);
    Material* GetMaterial();
};
}
