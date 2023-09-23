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

class VertexArray
{
public:
	MAYA_NO_COPY_CONSTRUCT(VertexArray);
	VertexArray(Primitives primitive = Primitives::Triangles);
	~VertexArray();

	// Link a vertex buffer object to this vao
	void LinkVBO(std::vector<float> const& data, VertexLayout layout);

	// Link a index buffer object to this vao
	void LinkIBO(std::vector<unsigned> const& data);

	void Draw();

private:
	unsigned int vaoid, iboid;
	std::vector<std::uint32_t> vboids;
	unsigned int attribloc;
	int vertex_count, elem_to_draw;
	Primitives primitives;

private:
	void Bind();
	void Unbind();
};

}