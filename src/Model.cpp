#include "Model.hpp"

#include "Mesh.hpp"
#include "Texture.hpp"
#include "ShadersManager.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string_view>
#include <exception>
#include <iostream>
#include <vector>

utils::Model::Model(std::string_view i_path) : d_directory(std::filesystem::path(i_path).parent_path())
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(i_path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		throw std::runtime_error("ERROR::Assimp: " + std::string(importer.GetErrorString()));
	}

	processNode(*scene->mRootNode, *scene);
}

void utils::Model::Draw(const utils::ShadersManager& i_shaders)
{
	for (auto& mesh : d_meshes)
		mesh.Draw(i_shaders);
}

void utils::Model::processNode(aiNode& i_node, const aiScene& i_scene)
{
	for (unsigned int i = 0; i < i_node.mNumMeshes; ++i)
	{
		aiMesh* mesh = i_scene.mMeshes[i_node.mMeshes[i]];
		if (!mesh)
		{
			std::cout << "Null mesh\n";
			continue;
		}

		d_meshes.push_back(processMesh(*mesh, i_scene));
	}

	for (unsigned int i = 0; i < i_node.mNumChildren; ++i)
	{
		auto* node = i_node.mChildren[i];
		if (!node)
		{
			std::cout << "Null node\n";
			continue;
		}

		processNode(*node, i_scene);
	}
}

utils::Mesh utils::Model::processMesh(aiMesh& i_mesh, const aiScene& i_scene)
{
	std::vector<utils::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<utils::Texture> textures;

	for (unsigned int i = 0; i < i_mesh.mNumVertices; ++i)
	{
		const auto vertex = i_mesh.mVertices[i];
		const glm::vec3 position{ vertex.x, vertex.y, vertex.z };

		const auto meshNormal = i_mesh.mNormals[i];
		if (!i_mesh.HasNormals())
		{
			std::cout << "Null meshNormal\n";
			return utils::Mesh(vertices, indices, textures);
		}
		const glm::vec3 normal{ meshNormal.x, meshNormal.y, meshNormal.z };

		const auto meshTexCoords = i_mesh.mTextureCoords[0];
		glm::vec2 texCoords{ 0.0f, 0.0f };
		if (meshTexCoords)
		{
			const auto meshUV = i_mesh.mTextureCoords[0][i];
			texCoords = glm::vec2{ meshUV.x, meshUV.y };
		}

		vertices.emplace_back(position, normal, texCoords);		
	}

	for (unsigned int i = 0; i < i_mesh.mNumFaces; ++i)
	{
		auto face = i_mesh.mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	if (i_mesh.mMaterialIndex >= 0)
	{
		auto* material = i_scene.mMaterials[i_mesh.mMaterialIndex];
		if (!material)
		{
			std::cout << "Null material\n";
			return utils::Mesh(vertices, indices, textures);
		}

		std::vector<utils::Texture> diffuseMaps = loadMaterialTextures(*material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<utils::Texture> specularMaps = loadMaterialTextures(*material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return utils::Mesh(vertices, indices, textures);
}

std::vector<utils::Texture> utils::Model::loadMaterialTextures(aiMaterial& i_material, aiTextureType i_textureType)
{
	std::vector<utils::Texture> textures;

	for (unsigned int i = 0; i < i_material.GetTextureCount(i_textureType); ++i)
	{
		aiString texPath;
		i_material.GetTexture(i_textureType, i, &texPath);

		const auto path = d_directory / texPath.C_Str();
		auto it = d_loadedTextures.find(path.string());
		if (it == d_loadedTextures.end())
		{
			const auto& texture = textures.emplace_back(path.string(), i_textureType);
			d_loadedTextures.insert({ path.string(), texture });
		}
		else
		{
			textures.push_back(it->second);
		}
	}

	return textures;
}
