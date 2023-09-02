#include "./private_control.hpp"

namespace Maya {

void VertexLayout::PushAttribute(int count)
{
	attributes.emplace_back(count, stride);
	stride += count;
}

VertexArray::VertexArray(int count, Primitives primitive)
	: iboid(0), attribloc(0), vertex_count(count), primitives(primitive), elem_to_draw(count)
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

void VertexArray::Bind()
{
	auto& ctrl = PrivateControl::Instance();
	if (this == ctrl.current_vao) return;
	ctrl.current_vao = this;
	glBindVertexArray(vaoid);
}

void VertexArray::Unbind()
{
	PrivateControl::Instance().current_vao = nullptr;
	glBindVertexArray(0);
}

VertexArray& VertexArray::LinkVBO(float const* data, VertexLayout layout)
{
	auto& vboid = vboids.emplace_back();
	Bind();
	glGenBuffers(1, &vboid);
	glBindBuffer(GL_ARRAY_BUFFER, vboid);
	glBufferData(GL_ARRAY_BUFFER, layout.stride * vertex_count * sizeof(float), data, GL_STATIC_DRAW);
	
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
	return *this;
}

VertexArray& VertexArray::LinkIBO(unsigned const* data, std::uint32_t count)
{
	Bind();
	if (iboid) glDeleteBuffers(1, &iboid);
	glGenBuffers(1, &iboid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	Unbind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	elem_to_draw = count;
	return *this;
}

void PrivateControl::ShaderDrawVAO(VertexArray& vao)
{
	vao.Bind();
	if (vao.iboid) glDrawElements((unsigned)vao.primitives, vao.elem_to_draw, GL_UNSIGNED_INT, 0);
	else glDrawArrays((unsigned)vao.primitives, 0, vao.vertex_count);
}

}