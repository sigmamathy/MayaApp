#pragma once

#include "../vertex_array.hpp"
#include "../shader.hpp"
#include "../texture.hpp"

namespace Maya {

class Model3D
{
public:
	Model3D(std::string const& path);
	~Model3D();

private:
	struct Mesh {
		VertexArray* vao;
		std::vector<Texture*> texture_ptrs;
		std::vector<std::string> texture_uniform_names;
	};

	std::string directory;
	std::vector<Mesh> meshes;
	std::unordered_map<std::string, Texture*> textures;

private:
	void ProcessNode(void* node, void const* scene);
	void ProcessMesh(void* mesh, void const* scene);
	void LoadMaterialTextures(Mesh& mesh, void* material, int type, std::string const& type_uniform);
	void Draw(Shader& shader) const;
	friend class Graphics3D;
};

}