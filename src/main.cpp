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
    util::ShadersManager objectShader("shaders/vertex.vs", "shaders/fragment.fs");
    util::ShadersManager lightShader("shaders/vertex.vs", "shaders/lightFragment.fs");
    util::TextureManager containerWooden("assets/container2.png");
    util::TextureManager containerBorder("assets/container2_specular.png");

    const auto cubeVertices = utils::getCubeWithNormalsAndTextures();
    const std::array cubePositions = utils::getCubesPositions();
    const std::array pointLightPositions = utils::getPointLightPos();

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightCubeVAO = 0;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::vec3 dirLightDir(0.2f, 1.0f, 0.3f);

    objectShader.render();
    // material properties
    objectShader.setInt("material.diffuse", 0);
    objectShader.setInt("material.specular", 1);
    // objectShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    objectShader.setFloat("material.shiness", 32);

    // light properties
    objectShader.setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    objectShader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    objectShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    objectShader.setVec3("dirLight.direction", -dirLightDir);

    /*objectShader.setFloat("light.constant", 1.0f);
    objectShader.setFloat("light.linear", 0.09f);
    objectShader.setFloat("light.quadratic", 0.032f);*/

    float deltaTime = 0.0f;
    float lastFrame = deltaTime;

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window))
    {
        process_input(window, camera, deltaTime, lastFrame);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectShader.render();

         //const float t = glfwGetTime();
         //lightDir.x = 2.0f * std::sin(t);
         //lightDir.y = -0.0f;
         //lightDir.z = 2.0f * std::cos(t);

        // positions
        objectShader.setVec3("viewPos", camera.getCameraPos());

        // view/projection transforms
        auto view = camera.getView();
        auto projection = camera.getProjection();
        objectShader.setMatrix4fv("view", view);
        objectShader.setMatrix4fv("projection", projection);

        // activating textures
        containerWooden.activate(GL_TEXTURE0);
        containerBorder.activate(GL_TEXTURE1);

        // world transform
        auto model = glm::mat4(1.0f);
        objectShader.setMatrix4fv("model", model);

        for (size_t i = 0; const auto& pos : pointLightPositions)
        {
            const std::string lightElem = "pointLights[" + std::to_string(i++) + "]";
            objectShader.setVec3(lightElem + ".position", pos);

            objectShader.setVec3(lightElem + ".ambient", glm::vec3(0.05f, 0.05f, 0.05f));
            objectShader.setVec3(lightElem + ".diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            objectShader.setVec3(lightElem + ".specular", glm::vec3(1.0f, 1.0f, 1.0f));

            objectShader.setFloat(lightElem + ".constant", 1.0f);
            objectShader.setFloat(lightElem + ".linear", 0.09f);
            objectShader.setFloat(lightElem + ".quadratic", 0.032f);
        }

        objectShader.setVec3("spotLight.position", camera.getCameraPos());
        objectShader.setVec3("spotLight.direction", camera.getCameraFront());
        objectShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        objectShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        objectShader.setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        objectShader.setVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        objectShader.setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        objectShader.setFloat("spotLight.constant", 1.0f);
        objectShader.setFloat("spotLight.linear", 0.09f);
        objectShader.setFloat("spotLight.quadratic", 0.032f);

        // render cube
        glBindVertexArray(objectVAO);
        /*glDrawArrays(GL_TRIANGLES, 0, 36);*/

        for (unsigned int i = 0; i < 10; ++i)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            const float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            objectShader.setMatrix4fv("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(lightCubeVAO);
        for (size_t i = 0; const auto& pos : pointLightPositions)
        {
            lightShader.render();
            lightShader.setMatrix4fv("view", view);
            lightShader.setMatrix4fv("projection", projection);

            model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setMatrix4fv("model", model);

            // render lamp object
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &objectVAO);
    // glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
