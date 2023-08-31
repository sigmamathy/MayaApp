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
	// Link a vertex buffer object to this vao
	// @param data: the array pointer that points the data,
	//				the size of array must equal to the indices count in the constructor
	// @param layout: specify the layout for the specific buffer
	VertexArray& LinkVBO(float const* data, VertexLayout layout);

	// Link a index buffer object to this vao
	// @param data: the array pointer that points to the indices data
	// @param count: the size of that array
	VertexArray& LinkIBO(unsigned const* data, std::uint32_t count);

private:
	unsigned int vaoid, iboid;
	std::vector<std::uint32_t> vboids;
	unsigned int attribloc;
	int vertex_count, elem_to_draw;
	Primitives primitives;

private:
	VertexArray(int count, Primitives primitive);
	VertexArray(VertexArray const&) = delete;
	VertexArray& operator=(VertexArray const&) = delete;
	~VertexArray();
	void Bind();
	friend class PrivateControl;
};

VertexArray& AssignVAO(std::string const& name, int count, Primitives primitive = Primitives::Triangles);

VertexArray& GetVAO(std::string const& name);

}