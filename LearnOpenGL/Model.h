#pragma once

#include <string>
#include <vector>
#include <map>

#include "assimp/scene.h"
#include "ShaderProgram.h"
#include "Mesh.h"

class Model
{
public:
	Model(std::string filePath) { loadModel(filePath); }
	
	void draw(std::shared_ptr<ShaderProgram> shaderProgram);
	void enableCulling(bool enabled);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::map<std::string, Texture> texturesLoaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(std::string path, std::string directory);
};

