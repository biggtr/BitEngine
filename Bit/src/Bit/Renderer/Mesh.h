#pragma once 

#include "Bit/Core/Defines.h"
#include "Bit/Math/Transform.h"
#include "Bit/Renderer/Material.h"
#include "Bit/Resources/MaterialManager.h"
#include "Bit/Resources/TextureManager.h"
#include "assimp/scene.h"
#include <string>
#include <vector>
namespace BitEngine
{
class Geometry;
class Mesh
{
private:
    std::vector<Geometry*> m_Geometries;
    BMath::Transform m_Transform;
    std::string m_Name;
    std::string m_Directory;
    MaterialManager* m_MaterialManager;
public:
    Mesh(MaterialManager* materialManager)
    {
        m_MaterialManager = materialManager;
    }
    ~Mesh();

    u32 GetGeometryCount();
    const std::vector<Geometry*>& GetGeometries() const;

    void SetName(const std::string& name);
    std::string GetName();

    void SetTransform(const BMath::Transform& t) { m_Transform = t; };
    BMath::Transform GetTransform() { return m_Transform; }

    void LoadModelFromFile(const std::string& filepath);
    void ProcessNode(aiNode* node, const aiScene* scene, aiMatrix4x4 parentTransform);
    Geometry* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    Material* ProcessMaterial(aiMaterial* aiMat, const aiScene* scene);
    void LoadMaterialTexture(aiMaterial* aiMat, aiTextureType, const std::string& textureName, Material* material);
    void DebugDumpMaterialTextures(aiMaterial* aiMat);
};
}
