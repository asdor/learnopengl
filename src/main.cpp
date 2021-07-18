#include <glad/glad.h> // should be included first

#include "CameraManager.hpp"
#include "ShadersManager.hpp"
#include "TextureManager.hpp"
#include "Vertices.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <array>

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window, util::Camera& io_camera, float& io_deltaTime, float& io_lastFrame)
{
    const float currentTime = glfwGetTime();
    io_deltaTime = currentTime - io_lastFrame;
    io_lastFrame = currentTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (io_camera.processKeyboard(window, io_deltaTime))
        return;
}

void mouse_callback(GLFWwindow* window, double i_xpos, double i_ypos)
{
    if (auto camera = reinterpret_cast<util::Camera*>(glfwGetWindowUserPointer(window)))
        camera->processMouseInput(i_xpos, i_ypos);
}

void scroll_callback(GLFWwindow* window, double i_xOffset, double i_yOffset)
{
    if (auto camera = reinterpret_cast<util::Camera*>(glfwGetWindowUserPointer(window)))
        camera->processScrollInput(i_xOffset, i_yOffset);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGl", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLWF window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    util::Camera camera(cameraPos, cameraFront, cameraUp);

    glfwSetWindowUserPointer(window, &camera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    stbi_set_flip_vertically_on_load(true);
    util::ShadersManager objectShader("../src/shaders/vertex.vs", "../src/shaders/fragment.fs");
    util::ShadersManager lightShader("../src/shaders/vertex.vs", "../src/shaders/lightFragment.fs");

    const auto cubeVertices = utils::getCubeWithNormals();
    // const std::array cubePositions = utils::getCubesPositions();

    unsigned int objectVAO = 0;
    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);

    // unsigned int EBO = 0;
    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cubeVertices.size(), cubeVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightCubeVAO = 0;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    objectShader.render();
    objectShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    objectShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    objectShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    objectShader.setFloat("material.shiness", 32);

    objectShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    objectShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    objectShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    float deltaTime = 0.0f;
    float lastFrame = deltaTime;

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window))
    {
        process_input(window, camera, deltaTime, lastFrame);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectShader.render();

        auto view = camera.getView();
        auto projection = camera.getProjection();

        // const float t = glfwGetTime();
        // lightPos.x = 2.0f * std::sin(t);
        // lightPos.y = -0.0f;
        // lightPos.z = 2.0f * std::cos(t);

        objectShader.setVec3("light.position", lightPos);
        objectShader.setMatrix4fv("view", view);
        objectShader.setMatrix4fv("projection", projection);
        objectShader.setVec3("viewPos", camera.getCameraPos());

        auto model = glm::mat4(1.0f);
        objectShader.setMatrix4fv("model", model);
        glBindVertexArray(objectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        lightShader.render();
        lightShader.setMatrix4fv("view", view);
        lightShader.setMatrix4fv("projection", projection);
        lightShader.setMatrix4fv("model", model);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &objectVAO);
    // glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
