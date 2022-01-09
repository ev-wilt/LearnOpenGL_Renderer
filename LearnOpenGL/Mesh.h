#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "ShaderProgram.h"

enum TextureType
{
	Diffuse,
	Specular
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	unsigned int id;
	TextureType type;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void draw(std::shared_ptr<ShaderProgram> shaderProgram);
	void enableCulling(bool enabled);

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int VAO, VBO, EBO;

	bool cullingEnabled;

	void setupMesh();
};