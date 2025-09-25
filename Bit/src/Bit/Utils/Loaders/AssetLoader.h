#pragma once 
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include <string>

namespace BitEngine
{
struct Mesh;
class Geometry;
Mesh LoadModelFromFile(const std::string& filepath);
std::vector<Geometry*> ProcessNode(aiNode* node, const aiScene* scene);
Geometry* ProcessMesh(aiMesh* mesh, const aiScene* scene);
}
