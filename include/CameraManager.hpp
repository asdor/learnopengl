#ifndef __CAMERA_MANAGER_HPP__
#define __CAMERA_MANAGER_HPP__

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace utils
{
class Camera
{
public:
    Camera(const glm::vec3& i_pos, const glm::vec3& i_front, const glm::vec3& i_up);

    void processMouseInput(double i_xPos, double i_yPos);
    void processScrollInput(double i_xOffset, double i_yOffset);
    bool processKeyboard(GLFWwindow* window, float i_deltaTime);

    glm::highp_mat4 getView() const;
    glm::highp_mat4 getProjection() const;
    glm::vec3 getCameraPos() const;

    glm::vec3 getCameraFront() const;

private:
    void updateCameraVectors();

    glm::vec3 d_pos;     // camera position
    glm::vec3 d_front;   // camera direction to target
    glm::vec3 d_up;      // camera up

    float d_yaw;         // yaw angle(left-right)
    float d_pitch;       // pitch angle(up-down)
    float d_fov;         // field of view
    glm::vec3 d_right;   // camera right
    glm::vec3 d_worldUp; // global world up

    bool isFirstMouse = true;
    double lastPosX = 0.0f;
    double lastPosY = 0.0f;
};
}

#endif // __CAMERA_MANAGER_HPP__
