#include "AssetLoader.h"
#include "Bit/Core/Logger.h"
#include "Bit/Renderer/Geometry.h"
#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/vector3.h"
#include <vector>

namespace BitEngine 
{

void LoadModelFromFile(Geometry* outGeometry, const std::string& filepath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode)
    {
        BIT_LOG_ERROR("Cannot Import scene with filepath : %s", filepath.c_str());
        BIT_LOG_ERROR("Error ASSIMP:: %s", importer.GetErrorString());
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

std::vector<Geometry> ProcessNode(aiNode* node, const aiScene* scene)
{

    std::vector<Geometry> geometries;
    for(u32 i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        geometries.push_back(ProcessMesh(mesh, scene));
    }
    for(u32 i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }

    return geometry; 
}

Geometry ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    Geometry* geometry = new Geometry(mesh->mName.C_Str());
    
    for(u32 i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vert;
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

    return geometry;
}

}
