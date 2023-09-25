#pragma once

#include "../vertex_array.hpp"
#include "../shader.hpp"
#include "../texture.hpp"

namespace Maya {

struct Mesh3D
{
	VertexArray* vao;
	std::vector<Texture*> texture_ptrs;
	std::vector<std::string> texture_uniform_names;
	void Draw(Shader& shader) const;
};

class Model3D
{
public:
	MAYA_NO_COPY_CONSTRUCT(Model3D);
	Model3D(std::string const& path);
	~Model3D();
	std::vector<Mesh3D> const& GetMeshes() const;

private:
	std::string directory;
	std::vector<Mesh3D> meshes;
	std::unordered_map<std::string, Texture*> textures;

private:
	void ProcessNode(void* node, void const* scene);
	void ProcessMesh(void* mesh, void const* scene);
	void LoadMaterialTextures(Mesh3D& mesh, void* material, int type, std::string const& type_uniform);
};

class Model3DAsyncLoader
{
public:
	Model3DAsyncLoader();
	void Add(Model3D** ptr, std::string const& path);

private:
	struct Target { Model3D** ptr; std::string path; void* scene; };
	std::vector<Target> targets;
};

}