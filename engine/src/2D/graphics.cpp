#include "../private_control.hpp"
#include <Maya2D.hpp>

namespace Maya {

constexpr static float square_vertices[] = {
	-0.5f, -0.5f, 0, 0,
	-0.5f, 0.5f, 0, 1,
	0.5f, -0.5f, 1, 0,
	-0.5f, 0.5f, 0, 1,
	0.5f, -0.5f, 1, 0,
	0.5f, 0.5f, 1, 1
};

void Graphics2D::InitResources()
{
	Shader* shader = new Shader("engine/res/Maya_2D.vert.glsl", "engine/res/Maya_2D.frag.glsl");
	shader->SetUniform("u_texture", 0);
	VertexArray* square_vao = new VertexArray(6);
	square_vao->LinkVBO(square_vertices, VertexLayout(2, 2));

	Assign("Maya_2D_shader", shader);
	Assign("Maya_2D_square_vao", square_vao);
}

Graphics2D::Graphics2D()
	: shader(GetShader("Maya_2D_shader")), rotation(0), oval_measure(0)
{
	SetColor(0xFFFFFF);
	SetTexture(nullptr);
	SetOvalGood(50);
	SetProjection(GetWindowSize());
	SetCameraPosition(0);
	SetCameraZoom(1);
}

void Graphics2D::SetProjection(Fvec2 size)
{
	SetProjection(size[0], size[1]);
}

void Graphics2D::SetProjection(float width, float height)
{
	Fmat4 proj = OrthogonalProjection(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f);
	shader.SetUniform("u_projection", proj);
}

void Graphics2D::SetCameraPosition(Fvec2 position)
{
	Fmat4 pos = Translate(-position);
	shader.SetUniform("u_view_pos", pos);
}

void Graphics2D::SetCameraPosition(float x, float y)
{
	SetCameraPosition(Fvec2(x, y));
}

void Graphics2D::SetCameraZoom(Fvec2 zoom)
{
	Fmat4 scale = Scale(zoom);
	shader.SetUniform("u_view_scale", scale);
}

void Graphics2D::SetCameraZoom(float x, float y)
{
	SetCameraZoom(Fvec2(x, y));
}

void Graphics2D::SetColor(unsigned int hex, float opacity)
{
	Fvec4 color;
	color[0] = (std::uint8_t)(hex >> 16) / 255.0f;
	color[1] = (std::uint8_t)(hex >> 8) / 255.0f;
	color[2] = (std::uint8_t)(hex) / 255.0f;
	color[3] = opacity;
	shader.SetUniform("u_color", color);
}

void Graphics2D::SetTexture(std::string const& name)
{
	SetTexture(&GetTexture(name));
}

void Graphics2D::SetTexture(Texture* texture)
{
	if (texture) texture->Bind(0);
	shader.SetUniform("u_draw_texture", texture ? 1 : 0);
}

void Graphics2D::SetRotation(float radian)
{
	rotation = radian;
}

void Graphics2D::SetOvalGood(unsigned int measure)
{
#if MAYA_DEBUG
	if (measure < 3) {
		std::cout << "Attempting to call SetOvalGood with measure < 3\n";
		return;
	}
#endif
	oval_measure = measure;
	auto& ctrl = PrivateControl::Instance();
	std::string name = "Maya_2D_oval_" + std::to_string(measure) + "_vao";
	if (ctrl.vaos.count(name)) return;

	std::vector<float> vertices;
	vertices.reserve(measure * 4);

	for (int i = 0; i < measure; i++)
	{
		float rot = 2 * 3.1415926536f * i / measure;
		float s = std::sinf(rot) * 0.5f, c = std::cosf(rot) * 0.5f;
		vertices.push_back(c);
		vertices.push_back(s);
		vertices.push_back(c + 0.5f);
		vertices.push_back(s + 0.5f);
	}

	unsigned int triangles = measure - 2;
	std::vector<unsigned int> indices;
	indices.reserve(triangles * 3);

	for (int i = 0; i < triangles; i++)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
	}

	VertexArray* oval_vao = new VertexArray(measure);
	oval_vao->LinkVBO(&vertices[0], VertexLayout(2, 2));
	oval_vao->LinkIBO(&indices[0], indices.size());
	Assign(name, oval_vao);
}

void Graphics2D::DrawRect(float x, float y, float width, float height)
{
	DrawRect(Fvec2(x, y), Fvec2(width, height));
}

void Graphics2D::DrawRect(Fvec2 position, Fvec2 scale)
{
	shader.SetUniform("u_model", Translate(position) * Rotate(rotation) * Scale(scale));
	shader.Draw("Maya_2D_square_vao");
}

void Graphics2D::DrawOval(float x, float y, float width, float height)
{
	DrawOval(Fvec2(x, y), Fvec2(width, height));
}

void Graphics2D::DrawOval(Fvec2 position, Fvec2 scale)
{
	shader.SetUniform("u_model", Translate(position) * Rotate(rotation) * Scale(scale));
	std::string name = "Maya_2D_oval_" + std::to_string(oval_measure) + "_vao";
	shader.Draw(name);
}

void Graphics2D::DrawLine(float x1, float y1, float x2, float y2)
{
	DrawLine(Fvec2(x1, y1), Fvec2(x2, y2));
}

void Graphics2D::DrawLine(Fvec2 start, Fvec2 end)
{
	Fmat4 pos = Translate((start + end) / 2.0f);
	Fvec2 dv = end - start;
	Fmat4 scale = Scale(Fvec2(dv.Norm(), 1.0f));
	Fmat4 rot = Rotate(std::atan2(dv[1], dv[0]));
	shader.SetUniform("u_model", pos * rot * scale);
	shader.Draw("Maya_2D_square_vao");
}

}