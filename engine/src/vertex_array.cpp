#include <Maya/vertex_array.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Maya {

void VertexLayout::PushAttribute(int count)
{
	attributes.emplace_back(count, stride);
	stride += count;
}

VertexArray::VertexArray(Primitives primitive)
	: iboid(0), attribloc(0), vertex_count(0), primitives(primitive), elem_to_draw(0)
{
	glGenVertexArrays(1, &vaoid);
	vboids.reserve(2);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &vaoid);
	for (auto& buffer : vboids)
		glDeleteBuffers(1, &buffer);
	if (iboid)
		glDeleteBuffers(1, &iboid);
}

static VertexArray* binded_vao = nullptr;

void VertexArray::Bind()
{
	if (this == binded_vao) return;
	binded_vao = this;
	glBindVertexArray(vaoid);
}

void VertexArray::Unbind()
{
	binded_vao = nullptr;
	glBindVertexArray(0);
}

void VertexArray::LinkVBO(std::vector<float> const& data, VertexLayout layout)
{
	auto& vboid = vboids.emplace_back();
	Bind();
#if MAYA_DEBUG
	if (vertex_count > 0 && vertex_count != data.size() / layout.stride) {
		MAYA_LOG("Error (VertexArray::LinkVBO): the number of vertices is inconsistence (this: "
			<< data.size() / layout.stride << ", previous: " << vertex_count << ")");
		return;
	}
#endif
	if (!vertex_count) {
		vertex_count = data.size() / layout.stride;
	}

	glGenBuffers(1, &vboid);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);
	glBufferData(GL_ARRAY_BUFFER, layout.stride * vertex_count * sizeof(float), data.data(), GL_STATIC_DRAW);

	for (auto i = 0u; i < layout.attributes.size(); i++)
	{
		glEnableVertexAttribArray(attribloc + i);
		auto const& attrib = layout.attributes[i];
		glVertexAttribPointer(attribloc + i, attrib.count, GL_FLOAT, false,
			layout.stride * sizeof(float), (void*)(attrib.offset * sizeof(float)));
	}

	attribloc += layout.attributes.size();
	Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::LinkIBO(std::vector<unsigned> const& data)
{
	Bind();
	if (iboid) glDeleteBuffers(1, &iboid);
	glGenBuffers(1, &iboid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), data.data(), GL_STATIC_DRAW);
	Unbind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	elem_to_draw = data.size();
}

void VertexArray::Draw()
{
#if MAYA_DEBUG
	if (!vertex_count) {
		MAYA_LOG("Warning (VertexArray::Draw): there is no vertex buffer being linked to this vertex array");
		return;
	}
#endif
	Bind();
	if (iboid) glDrawElements((unsigned)primitives, elem_to_draw, GL_UNSIGNED_INT, 0);
	else glDrawArrays((unsigned)primitives, 0, vertex_count);
}

}