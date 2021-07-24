#include "CameraManager.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string_view>

namespace
{
static constexpr float YAW = -90.0f;
static constexpr float PITCH = 0.0f;
static constexpr float SENSITIVITY = 0.1f;
static constexpr float FOV = 45.0f;

// void printVec3(std::string_view i_vecName, const glm::vec3& i_vec)
// {
//     std::cout << std::setw(6) << i_vecName << ": " << std::fixed << std::setprecision(2) << std::setw(6)
//               << i_vec.x << ' ' << i_vec.y << ' ' << i_vec.z << '\n';
// }
}

namespace util
{
Camera::Camera(const glm::vec3& i_pos, const glm::vec3& i_front, const glm::vec3& i_up)
    : d_pos(i_pos), d_front(i_front), d_up(i_up), d_yaw(YAW), d_pitch(PITCH), d_fov(FOV), d_worldUp(i_up)
{
    updateCameraVectors();
}

bool Camera::processKeyboard(GLFWwindow* window, float i_deltaTime)
{
    const float cameraSpeed = 2.5f * i_deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        d_pos += cameraSpeed * d_front;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        d_pos -= cameraSpeed * d_front;
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        d_pos -= cameraSpeed * d_right;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        d_pos += cameraSpeed * d_right;
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        d_pitch = std::clamp(d_pitch - 1.f, -89.f, 89.f);
        updateCameraVectors();
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        d_pitch = std::clamp(d_pitch + 1.f, -89.f, 89.f);
        updateCameraVectors();
    }

    return true;
}

void Camera::processScrollInput(double, double i_yOffset)
{
    d_fov -= static_cast<float>(i_yOffset);
    d_fov = std::clamp(d_fov, 1.0f, 45.0f);
}

void Camera::processMouseInput(double i_xPos, double i_yPos)
{

    if (isFirstMouse)
    {
        lastPosX = i_xPos;
        lastPosY = i_yPos;
        isFirstMouse = false;
    }

    float xOffset = SENSITIVITY * static_cast<float>(i_xPos - lastPosX);
    float yOffset = SENSITIVITY * static_cast<float>(i_yPos - lastPosY);

    lastPosX = i_xPos;
    lastPosY = i_yPos;

    d_yaw += xOffset;
    d_pitch += yOffset;

    d_pitch = std::clamp(d_pitch, -89.0f, 89.0f);
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    const auto radYaw = glm::radians(d_yaw);
    const auto radPitch = glm::radians(d_pitch);

    glm::vec3 front;
    front.x = std::cos(radYaw) * std::cos(radPitch);
    front.y = std::sin(-radPitch);
    front.z = std::sin(radYaw) * std::cos(radPitch);

    d_front = glm::normalize(front);
    d_right = glm::normalize(glm::cross(d_front, d_worldUp));
    d_up = glm::normalize(glm::cross(d_right, d_front));
}

glm::highp_mat4 Camera::getView() const
{
    return glm::lookAt(d_pos, d_pos + d_front, d_up);
}

glm::highp_mat4 Camera::getProjection() const
{
    return glm::perspective(glm::radians(d_fov), 800.f / 600.f, 0.1f, 100.f);
}

glm::vec3 Camera::getCameraPos() const
{
    return d_pos;
}
}
