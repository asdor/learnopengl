#include "Mesh.hpp"

#include "ShadersManager.hpp"
#include "Texture.hpp"

#include <glad/glad.h>

utils::Mesh::Mesh(const std::vector<utils::Vertex>& i_vertices, const std::vector<unsigned int>& i_indices, const std::vector<utils::Texture>& i_textures)
	: d_vertices(i_vertices), d_indices(i_indices), d_textures(i_textures)
{
	glGenVertexArrays(1, &d_VAO);
	glGenBuffers(1, &d_VBO);
	glGenBuffers(1, &d_EBO);

	glBindVertexArray(d_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, d_VBO);
	glBufferData(GL_ARRAY_BUFFER, d_vertices.size() * sizeof(Vertex), d_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, d_indices.size() * sizeof(unsigned int), d_indices.data(), GL_STATIC_DRAW);

	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(1 * sizeof(glm::vec3)));
	
	// texcoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));

	glBindVertexArray(0);
}

void utils::Mesh::Draw(const utils::ShadersManager& i_shaderManager)
{
	size_t diffuseCnt = 0;
	size_t specularCnt = 0;

	for (unsigned int i = 0; const auto& texture : d_textures)
	{
		texture.activate(GL_TEXTURE0 + i);

		size_t texNumber = 0;
		switch (texture.getType())
		{
		case aiTextureType::aiTextureType_DIFFUSE:
			texNumber = diffuseCnt++;
			break;
		case aiTextureType::aiTextureType_SPECULAR:
			texNumber = specularCnt++;
			break;
		default:
			break;
		}

		const auto textureName = "texture_" + texture.getTypeAsString() + std::to_string(texNumber);
		i_shaderManager.setFloat(textureName, static_cast<float>(i++));
	}

	glBindVertexArray(d_VAO);
	glDrawElements(GL_TRIANGLES, d_indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

utils::Mesh::~Mesh()
{
	/*glDeleteVertexArrays(1, &d_VAO);
	glDeleteBuffers(1, &d_EBO);
	glDeleteBuffers(1, &d_VBO);*/
}
