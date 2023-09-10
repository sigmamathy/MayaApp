#include "./private_control.hpp"

namespace Maya {

void VertexLayout::PushAttribute(int count)
{
	attributes.emplace_back(count, stride);
	stride += count;
}

static struct VAO_OpenGLResourceReleaser {
	std::vector<unsigned int> vaoids, bufferids;
	~VAO_OpenGLResourceReleaser() {
		for (auto id : vaoids) glDeleteVertexArrays(1, &id);
		for (auto id : bufferids) glDeleteBuffers(1, &id);
	}
} releaser;

static VertexArray* current_vao = nullptr;

VertexArray::VertexArray(VertexDataStruct& vds, int count, Primitives primitive)
	: VertexArray(vds, count, primitive, nullptr, 0)
{
}

VertexArray::VertexArray(VertexDataStruct& vds, int count, Primitives primitive, unsigned int* ibo, unsigned int ibosize)
	: iboid(0), attribloc(0), vertex_count(count), primitives(primitive), elem_to_draw(count)
{
	glGenVertexArrays(1, &vaoid);
	releaser.vaoids.push_back(vaoid);
	vboids.reserve(2);

	for (auto& [data, layout] : vds)
	{
		auto& vboid = vboids.emplace_back();
		releaser.bufferids.push_back(vaoid);

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
	}

	if (!ibo) return;
	glGenBuffers(1, &iboid);
	releaser.bufferids.push_back(iboid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibosize * sizeof(unsigned int), ibo, GL_STATIC_DRAW);
	Unbind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	elem_to_draw = ibosize;
}

void VertexArray::Bind()
{
	if (this == current_vao) return;
	current_vao = this;
	glBindVertexArray(vaoid);
}

void VertexArray::Unbind()
{
	current_vao = nullptr;
	glBindVertexArray(0);
}

void VertexArray::Draw()
{
	Bind();
	if (iboid) glDrawElements((unsigned)primitives, elem_to_draw, GL_UNSIGNED_INT, 0);
	else glDrawArrays((unsigned)primitives, 0, vertex_count);
}

}