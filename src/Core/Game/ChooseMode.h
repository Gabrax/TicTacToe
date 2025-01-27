#pragma once

#include "../Backend/Shader.h"
#include "../Backend/Util.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../Backend/window.h"
#include "../Backend/Util.hpp"

struct ChooseMode
{
    ChooseMode() = default;

    ~ChooseMode()
    {
        glDeleteBuffers(1, &StartVBO);
        glDeleteBuffers(1, &StartEBO);
        glDeleteVertexArrays(1, &StartVAO);

        glDeleteBuffers(1, &EnterVBO);
        glDeleteBuffers(1, &EnterEBO);
        glDeleteVertexArrays(1, &EnterVAO);
    }
    

    void BindAndLoad()
    {
        glGenVertexArrays(1, &StartVAO);
        glGenBuffers(1, &StartVBO);
        glGenBuffers(1, &StartEBO);
        glBindVertexArray(StartVAO);
        glBindBuffer(GL_ARRAY_BUFFER, StartVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TitleVertices), TitleVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, StartEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TitleIndices), TitleIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        ChooseModeTexture = Util::loadTexture("res/textures/pvp.png");

        glGenVertexArrays(1, &EnterVAO);
        glGenBuffers(1, &EnterVBO);
        glGenBuffers(1, &EnterEBO);
        glBindVertexArray(EnterVAO);
        glBindBuffer(GL_ARRAY_BUFFER, EnterVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(EnterVertices), EnterVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EnterEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(EnterIndices), EnterIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        PressEnterTexture = Util::loadTexture("res/textures/pve.png");
    }
    
    void Render(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f))
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ChooseModeTexture);

        _shader.Use();
        _shader.setVec3("viewPos", this->_camera.Position);
        glm::mat4 projection = glm::perspective(glm::radians(this->_camera.Zoom), Window::getAspectRatio(), 0.001f, 2000.0f);
        _shader.setMat4("projection", projection);
        _shader.setMat4("view", this->_camera.GetViewMatrix());
        // Model transformation
        Util::Transform transform;
        transform.position = position;
        transform.rotation = rotation;
        transform.scale = scale;

        _shader.setMat4("model", transform.to_mat4());
        _shader.setInt("texture1", 0);
        glBindVertexArray(StartVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, PressEnterTexture);

        _shader.setInt("texture1", 1);
        glBindVertexArray(EnterVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void Delete()
    {
       this->~ChooseMode();
    }

private:

    Shader& _shader = Util::g_shaders.basic;
    Camera& _camera = Window::_camera;
    unsigned int ChooseModeTexture;
    unsigned int StartVAO, StartVBO, StartEBO;

    float TitleVertices[20] = { 
        // Positions           // Texture Coordinates
        -0.5f, -1.0f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
         0.5f, -1.0f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
         0.5f, -0.5f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
        -0.5f, -0.5f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int TitleIndices[6] = {  
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };

    unsigned int PressEnterTexture;
    unsigned int EnterVAO, EnterVBO, EnterEBO;

    float EnterVertices[20] = { 
        // Positions           // Texture Coordinates
        -0.5f, 0.0f, 0.0f,    0.0f, 0.0f, // Vertex 1: Bottom-left
         0.5f, 0.0f, 0.0f,    1.0f, 0.0f, // Vertex 2: Bottom-right
         0.5f, 0.5f, 0.0f,    1.0f, 1.0f, // Vertex 3: Top-right
        -0.5f, 0.5f, 0.0f,    0.0f, 1.0f  // Vertex 4: Top-left
    };
    unsigned int EnterIndices[6] = {  
        0, 1, 2,   // first triangle
        2, 3, 0    // second triangle
    };
};
