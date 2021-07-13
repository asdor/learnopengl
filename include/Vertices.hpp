#ifndef __VERTICES_HPP__
#define __VERTICES_HPP__

#include <glm/glm.hpp>

#include <array>


namespace utils
{
std::array<float, 180> getCubeVertices();
std::array<glm::vec3, 10> getCubesPositions();
}

#endif // __VERTICES_HPP__
