#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "UtilsFwd.hpp"

#include <glm/glm.hpp>

#include <vector>

namespace utils
{
struct Vertex
{
	glm::vec3 d_position;
	glm::vec3 d_normal;
	glm::vec2 d_texCoords;
};

class Mesh
{
public:
	Mesh(const std::vector<utils::Vertex>& i_vertices, const std::vector<unsigned int>& i_indices, const std::vector<utils::Texture>& i_textures);
	void Draw(const utils::ShadersManager& i_shaderManager);
	~Mesh();

private:
	std::vector<utils::Vertex> d_vertices;
	std::vector<unsigned int> d_indices;
	std::vector<utils::Texture> d_textures;

	unsigned int d_VAO = 0;
	unsigned int d_VBO = 0;
	unsigned int d_EBO = 0;
};
}

#endif //__MESH_HPP__
