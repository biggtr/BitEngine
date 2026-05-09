#include "Mesh.h"
#include "Bit/Renderer/Geometry.h"
#include "Bit/Renderer/Material.h"
#include "Bit/Resources/AssetStore.h"
#include "Bit/Resources/MaterialManager.h"
#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/matrix4x4.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include <filesystem>
namespace BitEngine
{
static BMath::Mat4 AiMatToMat4(const aiMatrix4x4& m)
{
    BMath::Mat4 result;
    result.Data[0]  = m.a1; result.Data[1]  = m.b1; result.Data[2]  = m.c1; result.Data[3]  = m.d1;
    result.Data[4]  = m.a2; result.Data[5]  = m.b2; result.Data[6]  = m.c2; result.Data[7]  = m.d2;
    result.Data[8]  = m.a3; result.Data[9]  = m.b3; result.Data[10] = m.c3; result.Data[11] = m.d3;
    result.Data[12] = m.a4; result.Data[13] = m.b4; result.Data[14] = m.c4; result.Data[15] = m.d4;
    return result;
}

Mesh::~Mesh()
{
    for(auto& geometry : m_Geometries)
    {
        delete geometry;
    }
}
const std::vector<Geometry*>& Mesh::GetGeometries() const
{
    return m_Geometries;
}
u32 Mesh::GetGeometryCount()
{
    return m_Geometries.size();
}

void Mesh::SetName(const std::string& name)
{
    m_Name = name;
}
std::string Mesh::GetName()
{
    return m_Name;
}
void Mesh::LoadModelFromFile(const std::string& filepath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_GenNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        BIT_LOG_ERROR("Cannot Import scene with filepath : %s", filepath.c_str());
        BIT_LOG_ERROR("Error ASSIMP:: %s", importer.GetErrorString());
        return;
    }
    size_t slash = filepath.find_last_of("/\\");
    m_Directory = (slash == std::string::npos) ? "." : filepath.substr(0, slash);
    ProcessNode(scene->mRootNode, scene, aiMatrix4x4());
}

void Mesh::ProcessNode(aiNode* node, const aiScene* scene, aiMatrix4x4 parentTransform)
{
    aiMatrix4x4 globalTransform = parentTransform * node->mTransformation;
    for(u32 i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Geometry* geo = ProcessMesh(mesh, scene);
        geo->SetTransform(AiMatToMat4(globalTransform));
        m_Geometries.push_back(geo);
    }
    for(u32 i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene, globalTransform);
    }
}

Geometry* Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    Geometry* geometry = new Geometry(mesh->mName.C_Str());
    
    for(u32 i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vert;
        vert.Color = {1,1,1,1};
        aiVector3D v = mesh->mVertices[i];
        vert.Position = {v.x, v.y, v.z};
        if(mesh->HasNormals())
        {
            aiVector3D n = mesh->mNormals[i];
            vert.Normal = {n.x, n.y, n.z};
        }
        if(mesh->HasTextureCoords(0))
        {
            aiVector3D t = mesh->mTextureCoords[0][i];
            vert.TextureCoords = {t.x, t.y};
        }
        vertices.push_back(vert);
    }
    for(u32 i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace* f = &mesh->mFaces[i];
        for(u32 j = 0; j < f->mNumIndices; ++j)
        {
            indices.push_back(f->mIndices[j]);
        }
    }
    geometry->SetVertices(vertices);
    geometry->SetIndices(indices);
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
        Material* material = ProcessMaterial(aiMat, scene);
        geometry->SetMaterial(material);
    }
    return geometry;
}
Material* Mesh::ProcessMaterial(aiMaterial* aiMat, const aiScene* scene)
{
    aiString matName;
    aiMat->Get(AI_MATKEY_NAME, matName);
    Material* material = m_MaterialManager->CreateMaterial(matName.C_Str(), "phongShader");
    LoadMaterialTexture(aiMat, aiTextureType_DIFFUSE,  "u_DiffuseMap",  material);
    // LoadMaterialTexture(aiMat, aiTextureType_BASE_COLOR,  "u_DiffuseMap",  material);
    LoadMaterialTexture(aiMat, aiTextureType_SPECULAR, "u_SpecularMap", material);
    LoadMaterialTexture(aiMat, aiTextureType_NORMALS,  "u_NormalMap",   material);
    LoadMaterialTexture(aiMat, aiTextureType_HEIGHT,   "u_HeightMap",   material);
    return material;
}
void Mesh::LoadMaterialTexture(aiMaterial* aiMat, aiTextureType type,
                                const std::string& uniformName, Material* material)
{
    if (aiMat->GetTextureCount(type) == 0) return;

    aiString path;
    aiMat->GetTexture(type, 0, &path);
    std::string texPath = path.C_Str(); // "textures/UV_mat_baseColor.jpeg"

    std::string fullPath = m_Directory + "/" + texPath;

    if (!std::filesystem::exists(fullPath))
    {
        size_t slash = texPath.find_last_of("/\\");
        std::string filename = (slash != std::string::npos) 
                             ? texPath.substr(slash + 1) 
                             : texPath;
        fullPath = m_Directory + "/" + filename;
        BIT_LOG_WARN("[%s] Path not found, trying filename only: '%s'", 
                     uniformName.c_str(), fullPath.c_str());
    }

    BIT_LOG_DEBUG("[%s] Loading texture: '%s'", uniformName.c_str(), fullPath.c_str());

    Texture* tex = AssetStoreAddTexture(fullPath.c_str(), fullPath.c_str());
    if (!tex)
    {
        BIT_LOG_ERROR("[%s] FAILED: '%s'", uniformName.c_str(), fullPath.c_str());
        return;
    }
    material->SetTexture(uniformName, tex);
}

}
