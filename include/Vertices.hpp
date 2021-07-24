#ifndef __VERTICES_HPP__
#define __VERTICES_HPP__

#include <glm/glm.hpp>

#include <array>
#include <vector>

namespace utils
{
std::array<float, 180> getCubeVertices();
std::array<glm::vec3, 10> getCubesPositions();
std::vector<float> getCubeWithNormals();
std::array<float, 288> getCubeWithNormalsAndTextures();
}

#endif // __VERTICES_HPP__
