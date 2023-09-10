#include "../private_control.hpp"
#include <Maya3D.hpp>

namespace Maya {

static constexpr float cube_vertices[] =
{
	// front
	-0.5f, -0.5f, -0.5f,	0, 0, -1,		0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0, 0, -1,		0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		0, 0, -1,		1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0, 0, -1,		1.0f, 1.0f,

	// back
	-0.5f, -0.5f, 0.5f,		0, 0, 1,		1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0, 0, 1,		1.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		0, 0, 1,		0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0, 0, 1,		0.0f, 1.0f,

	// left
	-0.5f, 0.5f, 0.5f,		-1, 0, 0,		0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		-1, 0, 0,		0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		-1, 0, 0,		1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1, 0, 0,		1.0f, 0.0f,

	// right
	0.5f, 0.5f, 0.5f,		1, 0, 0,		1.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		1, 0, 0,		1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1, 0, 0,		0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		1, 0, 0,		0.0f, 0.0f,

	// top
	-0.5f, 0.5f, -0.5f,		0, 1, 0,		0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0, 1, 0,		0.0f, 1.0f,
	0.5f, 0.5f, -0.5f,		0, 1, 0,		1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0, 1, 0,		1.0f, 1.0f,

	// bottom
	-0.5f, -0.5f, -0.5f,	0, 1, 0,		0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		0, 1, 0,		0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0, 1, 0,		1.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		0, 1, 0,		1.0f, 0.0f,
};

static constexpr unsigned int cube_indices[] =
{
	0, 1, 2,
	1, 2, 3,
	4, 5, 6,
	5, 6, 7,
	8, 9, 10,
	9, 10, 11,
	12, 13, 14,
	13, 14, 15,
	16, 17, 18,
	17, 18, 19,
	20, 21, 22,
	21, 22, 23
};

void Graphics3D::InitResources()
{
	Shader* shader = new Shader("engine/res/3D/shaders/default.vert.glsl", "engine/res/3D/shaders/default.frag.glsl");

	VertexArray* cube_vao = new VertexArray(24);
	cube_vao->LinkVBO(cube_vertices, VertexLayout(3, 3, 2));
	cube_vao->LinkIBO(cube_indices, 36);

	Assign("Maya_3D_shader_default", shader);
	Assign("Maya_3D_vao_cube", cube_vao);
}

Graphics3D::Graphics3D() : shader(GetShader("Maya_3D_shader_default"))
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
}

float rot = 0.0f;

void Graphics3D::DrawCube(float elapsed)
{
	rot += elapsed;
	shader.SetUniform("u_projection", PerspectiveProjection(3.1415926536f / 3, 16.0f / 9, 0.1f, 100.0f));
	shader.SetUniform("u_view", LookAt(Fvec3(0.0f, -2.0f, -2.0f), Fvec3(0.0f, 1.0f, 1.0f)));
	shader.SetUniform("u_model", Rotate(rot, up));

	GetTexture("Maya").Bind(0);
	shader.SetUniform("u_texture", 0);

	shader.Draw("Maya_3D_vao_cube");
}

}