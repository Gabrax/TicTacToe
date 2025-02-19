#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <meshoptimizer.h>

#include "Shader.h"

#include <string>
#include <vector>
#include <array>
#include <unordered_map>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct Mesh {

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    GLuint VAO;


    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures)
        : vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures))
    {
        optimizeMesh();
        setupMesh();
    }

    void Draw(const Shader& shader) {
        std::unordered_map<std::string, GLuint> textureCounters = {
            {"texture_diffuse", 1},
            {"texture_specular", 1},
            {"texture_normal", 1},
            {"texture_height", 1}
        };

        for (GLuint i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string number = std::to_string(textureCounters[textures[i].type]++);
            shader.setInt(textures[i].type + number, i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // Draw the mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }

private:
    GLuint VBO, EBO;


    void optimizeMesh()
    {
        meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), vertices.size());

        meshopt_optimizeOverdraw(
            indices.data(), 
            indices.data(), 
            indices.size(), 
            &vertices[0].Position.x, 
            vertices.size(), 
            sizeof(Vertex), 
            1.05f // Overdraw threshold (1.0 = minimal overdraw)
        );

        std::vector<Vertex> optimizedVertices(vertices.size());
        meshopt_optimizeVertexFetch(
            optimizedVertices.data(), 
            indices.data(), 
            indices.size(), 
            vertices.data(), 
            vertices.size(), 
            sizeof(Vertex)
        );

        vertices = std::move(optimizedVertices);
    }

    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Load data into vertex buffers
        bindAndBufferData(GL_ARRAY_BUFFER, VBO, vertices.size() * sizeof(Vertex), &vertices[0]);
        bindAndBufferData(GL_ELEMENT_ARRAY_BUFFER, EBO, indices.size() * sizeof(unsigned int), &indices[0]);

        // Set up vertex attributes
        setupVertexAttributes();

        glBindVertexArray(0);
    }

    void bindAndBufferData(GLenum bufferType, unsigned int& buffer, GLsizeiptr size, const void* data)
    {
        glBindBuffer(bufferType, buffer);
        glBufferData(bufferType, size, data, GL_STATIC_DRAW);
    }

    void setupVertexAttributes()
    {
        struct Attribute {
            GLint size;
            GLenum type;
            GLboolean normalized;
            size_t offset;
        };

        std::array<Attribute, 7> attributes = { {
            {3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position)},
            {3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal)},
            {2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCoords)},
            {3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Tangent)},
            {3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Bitangent)},
            {4, GL_INT, GL_FALSE, offsetof(Vertex, m_BoneIDs)},
            {4, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_Weights)}
        } };

        for (size_t i = 0; i < attributes.size(); ++i) {
            glEnableVertexAttribArray(static_cast<GLuint>(i));
            if (attributes[i].type == GL_INT) {
                glVertexAttribIPointer(static_cast<GLuint>(i), attributes[i].size, attributes[i].type, sizeof(Vertex), (void*)attributes[i].offset);
            } else {
                glVertexAttribPointer(static_cast<GLuint>(i), attributes[i].size, attributes[i].type, attributes[i].normalized, sizeof(Vertex), (void*)attributes[i].offset);
            }
        }
    }
};
