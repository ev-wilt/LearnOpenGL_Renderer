#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
    vertices(vertices), indices(indices), textures(textures), cullingEnabled(true)
{
    setupMesh();
}

void Mesh::draw(std::shared_ptr<ShaderProgram> shaderProgram)
{
    //unsigned int diffuseCount = 1;
    //unsigned int specularCount = 1;
    //for (unsigned int i = 0; i < textures.size(); i++)
    //{
    //    glActiveTexture(GL_TEXTURE0 + i);

    //    std::string number;
    //    std::string name;

    //    if (textures[i].type == TextureType::Diffuse)
    //    {
    //        name = "texture_diffuse";
    //        number = std::to_string(diffuseCount++);
    //    }
    //    else if (textures[i].type == TextureType::Specular)
    //    {
    //        name = "texture_specular";
    //        number = std::to_string(specularCount++);
    //    }

    //    shaderProgram->setFloat(("material." + name + number).c_str(), static_cast<float>(i));
    //    glBindTexture(GL_TEXTURE_2D, textures[i].id);
    //}
    //glActiveTexture(GL_TEXTURE0);

    unsigned int diffuseCount = 1;
    unsigned int specularCount = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name;

        if (textures[i].type == TextureType::Diffuse)
        {
            name = "diffuse";
        }
        else if (textures[i].type == TextureType::Specular)
        {
            name = "specular";
        }

        shaderProgram->setInt(("material." + name).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    if (cullingEnabled)
    {
        glEnable(GL_CULL_FACE);
    }

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (cullingEnabled)
    {
        glDisable(GL_CULL_FACE);
    }

}

void Mesh::enableCulling(bool enabled)
{
    cullingEnabled = enabled;
}

void Mesh::setupMesh()
{
    // Setup objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind objects and copy data to them
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // Define layout of vertex data
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}
