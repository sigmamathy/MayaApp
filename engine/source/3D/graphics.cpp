#include <Maya/3D/graphics.hpp>
#include <Maya/shader.hpp>
#include <Maya/transformation.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Maya {

static bool is_g3d_initialized = false;
static Shader* shader;

static VertexArray* cube_vao;

static std::vector<float> cube_vertices =
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

static std::vector<unsigned> cube_indices =
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
	if (is_g3d_initialized) return;
	shader = new Shader("engine/resource/3D/shaders/default.vert.glsl", "engine/resource/3D/shaders/default.frag.glsl", true);

	cube_vao = new VertexArray;
	cube_vao->LinkVBO(cube_vertices, VertexLayout(3, 3, 2));
	cube_vao->LinkIBO(cube_indices);

	is_g3d_initialized = true;
}

void Graphics3D::FreeResources()
{
	if (!is_g3d_initialized) return;
	delete shader;
	delete cube_vao;
	is_g3d_initialized = false;
}

Graphics3D::Graphics3D()
{
	InitResources();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
}

static float rotate = 0.0f;

void Graphics3D::Draw(Model3D const& model)
{
	rotate += 0.01f;

	shader->SetUniform("u_projection", PerspectiveProjection(3.1415926536f / 2, 16.0f / 9, 0.1f, 100.0f));
	shader->SetUniform("u_view", LookAt(Fvec3(0.0f, 0.0f, -10.0f), Fvec3(0.0f, 0.0f, 1.0f)));
	shader->SetUniform("u_model", Rotate(rotate, Fvec3(0.0f, 1.0f, 0.0f)));

	model.Draw(*shader);
}

void Graphics3D::DrawCube()
{
	shader->SetUniform("u_projection", PerspectiveProjection(3.1415926536f / 2, 16.0f / 9, 0.1f, 100.0f));
	shader->SetUniform("u_view", LookAt(Fvec3(0.0f, 0.0f, -2.0f), Fvec3(0.0f, 0.0f, 1.0f)));
	shader->SetUniform("u_model", Fmat4(1.0f));

	cube_vao->Draw();
}

}