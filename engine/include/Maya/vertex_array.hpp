#pragma once

#include "./core.hpp"

namespace Maya {

// Vertex array draw primitives option
enum class Primitives : std::uint8_t
{
	Points,
	Lines,
	LineLoop,
	LineStrip,
	Triangles,
	TriangleStrip,
	TriangleFan
};

// Vertex array layout, calculates offsets and the stride. 
// Increments the attribute position as it goes
struct VertexLayout final
{
	// Initialize the layout
	// @param args: each indicates the count of the attribute
	template<class... Tys> requires (std::is_same_v<Tys, int> && ...)
	VertexLayout(Tys... args) { (PushAttribute(args), ...); }

	// Add a vertex attribute
	void PushAttribute(int count);

	// Vertex attributes
	struct Attribute final { int count, offset; };
	std::vector<Attribute> attributes;
	int stride = 0;
};

using VertexDataStruct = std::vector<std::pair<float*, VertexLayout>>;

class VertexArray
{
public:
	void Bind();
	void Unbind();
	void Draw();

private:
	unsigned int vaoid, iboid;
	std::vector<std::uint32_t> vboids;
	unsigned int attribloc;
	int vertex_count, elem_to_draw;
	Primitives primitives;

private:
	VertexArray(VertexDataStruct& vds, int count, Primitives primitive = Primitives::Triangles);
	VertexArray(VertexDataStruct& vds, int count, Primitives primitive, unsigned int* ibo, unsigned int ibosize);
	friend class ResourcesManager;
};

}