#include "../private_control.hpp"
#include <Maya2D.hpp>

namespace Maya {

constexpr static float square_vertices[] = {
	-0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f
};

void LoadResources2D()
{
	AssignShader("Maya_2D_shader", "engine/res/Maya_2D.vert.glsl", "engine/res/Maya_2D.frag.glsl");
	AssignVAO("Maya_2D_square_vao", 6).LinkVBO(square_vertices, VertexLayout(2));
}

Renderer2D::Renderer2D(Scene2D* parent)
	: parent(parent), rotation(0)
{
	UpdateViewAndProjection();
	SetColor(0);
	SetOvalGood(50);
}

void Renderer2D::UpdateViewAndProjection()
{
	auto& shader = GetShader("Maya_2D_shader");
	shader.SetUniform("u_projection", parent->projection);
	shader.SetUniform("u_view", parent->camera_scale * parent->camera_pos);
}

void Renderer2D::SetColor(unsigned int hex, float opacity)
{
	auto& shader = GetShader("Maya_2D_shader");
	Fvec4 color;
	color[0] = (std::uint8_t)(hex >> 16) / 255.0f;
	color[1] = (std::uint8_t)(hex >> 8) / 255.0f;
	color[2] = (std::uint8_t)(hex) / 255.0f;
	color[3] = opacity;
	shader.SetUniform("u_color", color);
}

void Renderer2D::SetRotation(float radian)
{
	rotation = radian;
}

void Renderer2D::SetOvalGood(unsigned int measure)
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
	vertices.reserve(measure);
	for (int i = 0; i < measure; i++) {
		float rot = 2 * 3.1415926536f * i / measure;
		vertices.push_back(std::cosf(rot));
		vertices.push_back(std::sinf(rot));
	}

	unsigned int triangles = measure - 2;
	std::vector<unsigned int> indices;
	indices.reserve(triangles * 3);
	for (int i = 0; i < triangles; i++) {
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
	}

	AssignVAO(name, measure)
		.LinkVBO(&vertices[0], VertexLayout(2))
		.LinkIBO(&indices[0], indices.size());
}

void Renderer2D::DrawRect(int x, int y, int width, int height)
{
	DrawRect(Fvec2(x, y), Fvec2(width, height));
}

void Renderer2D::DrawRect(Fvec2 position, Fvec2 scale)
{
	auto& shader = GetShader("Maya_2D_shader");
	shader.SetUniform("u_model", Translate(position) * Rotate(rotation) * Scale(scale));
	shader.Draw("Maya_2D_square_vao");
}

void Renderer2D::DrawOval(int x, int y, int width, int height)
{
	DrawOval(Fvec2(x, y), Fvec2(width, height));
}

void Renderer2D::DrawOval(Fvec2 position, Fvec2 scale)
{
	auto& shader = GetShader("Maya_2D_shader");
	shader.SetUniform("u_model", Translate(position) * Rotate(rotation) * Scale(scale));
	std::string name = "Maya_2D_oval_" + std::to_string(oval_measure) + "_vao";
	shader.Draw(name);
}

Scene2D::Scene2D()
{
	SetProjection(GetWindowSize());
	SetCameraPosition(0);
	SetCameraZoom(1);
}

void Scene2D::SetProjection(Fvec2 size)
{
	projection = OrthogonalProjection(-size[0] / 2.0f, size[0] / 2.0f, -size[1] / 2.0f, size[1] / 2.0f);
}

void Scene2D::SetCameraPosition(Fvec2 position)
{
	camera_pos = Translate(-position);
}

void Scene2D::SetCameraZoom(Fvec2 size)
{
	camera_scale = Scale(size);
}

Renderer2D Scene2D::CreateRenderer()
{
	return Renderer2D(this);
}

}