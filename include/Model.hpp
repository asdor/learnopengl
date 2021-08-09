#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include "UtilsFwd.hpp"

#include <assimp/scene.h>


#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include "unordered_map"

namespace utils
{
class Model
{
public:
	Model(std::string_view i_path);
	void Draw(const utils::ShadersManager& i_shaders);

private:
	std::vector<utils::Mesh> d_meshes;
	std::filesystem::path d_directory;
	std::unordered_map<std::string, utils::Texture> d_loadedTextures;

	void processNode(aiNode& i_node, const aiScene& i_scene);
	utils::Mesh processMesh(aiMesh& i_mesh, const aiScene& i_scene);

	std::vector<utils::Texture> loadMaterialTextures(aiMaterial& i_material, aiTextureType i_textureType);
};
}

#endif //__MODEL_HPP__
