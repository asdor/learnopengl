#include <glad/glad.h> // should be included first

#include "CameraManager.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "ShadersManager.hpp"
#include "Texture.hpp"
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

void process_input(GLFWwindow* window, utils::Camera& io_camera, float& io_deltaTime, float& io_lastFrame)
{
    const float currentTime = static_cast<float>(glfwGetTime());
    io_deltaTime = currentTime - io_lastFrame;
    io_lastFrame = currentTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (io_camera.processKeyboard(window, io_deltaTime))
        return;
}

void mouse_callback(GLFWwindow* window, double i_xpos, double i_ypos)
{
    if (auto camera = reinterpret_cast<utils::Camera*>(glfwGetWindowUserPointer(window)))
        camera->processMouseInput(i_xpos, i_ypos);
}

void scroll_callback(GLFWwindow* window, double i_xOffset, double i_yOffset)
{
    if (auto camera = reinterpret_cast<utils::Camera*>(glfwGetWindowUserPointer(window)))
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
    utils::Camera camera(cameraPos, cameraFront, cameraUp);

    glfwSetWindowUserPointer(window, &camera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    stbi_set_flip_vertically_on_load(true);

    utils::ShadersManager modelShader("shaders/vertex.vs", "shaders/model_loading.fs");

    utils::Model modelLoader("../../../backpack/backpack.obj");

    glm::vec3 dirLightDir(0.2f, 1.0f, 0.3f);

    float deltaTime = 0.0f;
    float lastFrame = deltaTime;

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window))
    {
        process_input(window, camera, deltaTime, lastFrame);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelShader.render();

         //const float t = glfwGetTime();
         //lightDir.x = 2.0f * std::sin(t);
         //lightDir.y = -0.0f;
         //lightDir.z = 2.0f * std::cos(t);

        // positions

        // view/projection transforms
        auto view = camera.getView();
        auto projection = camera.getProjection();
        modelShader.setMatrix4fv("view", view);
        modelShader.setMatrix4fv("projection", projection);

        // world transform
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        modelShader.setMatrix4fv("model", model);
        modelLoader.Draw(modelShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
