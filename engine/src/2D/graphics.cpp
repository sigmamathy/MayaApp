#include <Maya/2D/graphics.hpp>
#include <Maya/shader.hpp>
#include <Maya/vertex_array.hpp>
#include <Maya/font.hpp>
#include <Maya/window.hpp>
#include <Maya/transformation.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Maya {

static bool is_g2d_initialized = false;
static Shader* shader = nullptr;

static VertexArray* square_vao;
static std::unordered_map<unsigned, VertexArray*> oval_vaos;
static Font* default_font;
static std::unordered_map<std::string, Texture*> glows;

static std::vector<float> square_vertices = {
	-0.5f, -0.5f, 0, 0,
	-0.5f, 0.5f, 0, 1,
	0.5f, -0.5f, 1, 0,
	-0.5f, 0.5f, 0, 1,
	0.5f, -0.5f, 1, 0,
	0.5f, 0.5f, 1, 1
};

void Graphics2D::InitResources()
{
	if (is_g2d_initialized) return;

	shader = new Shader("engine/res/2D/shaders/default.vert.glsl", "engine/res/2D/shaders/default.frag.glsl");
	shader->SetUniform("u_texture", 0);
	shader->SetUniform("u_glow_texture", 1);

	square_vao = new VertexArray;
	square_vao->LinkVBO(square_vertices, VertexLayout(2, 2));
	default_font = new Font("engine/res/2D/fonts/Arial.ttf", 30);

#define MAYA_GLOW(_name) glows[_name] = new Texture("engine/res/2D/glows/" _name ".jpg", 3)
	MAYA_GLOW("horizontal");
	MAYA_GLOW("vertical");
	MAYA_GLOW("diagonal");
	MAYA_GLOW("quarter_circle");
	MAYA_GLOW("quarter_circle_exclusive");
	MAYA_GLOW("center");
	MAYA_GLOW("center_exclusive");
#undef MAYA_GLOW

	is_g2d_initialized = true;
}

void Graphics2D::FreeResources()
{
	if (!is_g2d_initialized) return;
	delete shader;
	delete square_vao;
	for (auto pair : oval_vaos) delete pair.second;
	delete default_font;
	for (auto pair : glows) delete pair.second;
	is_g2d_initialized = false;
}

Graphics2D::Graphics2D()
	: rotation(0), oval_measure(0), line_width(1)
{
	InitResources();
	SetColor(0xFFFFFF);
	SetTexture(nullptr);
	SetGlowDirection(NoGlow);
	SetOvalGood(50);
	SetFont(nullptr);
	SetTextAlignment(AlignLeft);

	SetProjection(GameWindow::GetInstance().GetSize());
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
	shader->SetUniform("u_projection", proj);
}

void Graphics2D::SetCameraPosition(Fvec2 position)
{
	Fmat4 pos = Translate(-position);
	shader->SetUniform("u_view_pos", pos);
}

void Graphics2D::SetCameraPosition(float x, float y)
{
	SetCameraPosition(Fvec2(x, y));
}

void Graphics2D::SetCameraZoom(Fvec2 zoom)
{
	Fmat4 scale = Scale(zoom);
	shader->SetUniform("u_view_scale", scale);
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
	shader->SetUniform("u_color", color);
}

void Graphics2D::SetTexture(Texture* texture)
{
	if (texture) texture->Bind(0);
	shader->SetUniform("u_draw_texture", texture ? 1 : 0);
}

void Graphics2D::SetGlowDirection(GlowDirection dir)
{
	std::string target;
	switch (dir)
	{
		case GlowHorizontal:				target = "horizontal"; break;
		case GlowVertical:					target = "vertical"; break;
		case GlowDiagonal:					target = "diagonal"; break;
		case GlowQuarterCircle:				target = "quarter_circle"; break;
		case GlowQuarterCircleExclusive:	target = "quarter_circle_exclusive"; break;
		case GlowCenter:					target = "center"; break;
		case GlowCenterExclusive:			target = "center_exclusive"; break;
		default: shader->SetUniform("u_draw_glow", 0); return;
	}

	glows.at(target)->Bind(1);
	shader->SetUniform("u_draw_glow", 1);
}

void Graphics2D::SetRotation(float radian)
{
	rotation = radian;
}

void Graphics2D::SetOvalGood(unsigned int measure)
{
#if MAYA_DEBUG
	if (measure < 3) {
		MAYA_LOG("Warning (Graphics2D::SetOvalGood): measure is less than 3");
		return;
	}
#endif
	oval_measure = measure;
	if (oval_vaos.count(measure)) return;

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

	VertexArray* vao = new VertexArray;
	vao->LinkVBO(vertices, VertexLayout(2, 2));
	vao->LinkIBO(indices);
	oval_vaos[measure] = vao;
}

void Graphics2D::SetLineWidth(float width)
{
	line_width = width;
}

void Graphics2D::SetFont(Font* font)
{
	this->font = font ? font : default_font;
}

void Graphics2D::SetTextAlignment(TextAlignment align)
{
	this->align = align;
}

void Graphics2D::DrawRect(float x, float y, float width, float height)
{
	DrawRect(Fvec2(x, y), Fvec2(width, height));
}

void Graphics2D::DrawRect(Fvec2 position, Fvec2 scale)
{
	shader->SetUniform("u_model", Translate(position) * Rotate(rotation) * Scale(scale));
	square_vao->Draw();
}

void Graphics2D::DrawOval(float x, float y, float width, float height)
{
	DrawOval(Fvec2(x, y), Fvec2(width, height));
}

void Graphics2D::DrawOval(Fvec2 position, Fvec2 scale)
{
	shader->SetUniform("u_model", Translate(position) * Rotate(rotation) * Scale(scale));
	oval_vaos.at(oval_measure)->Draw();
}

void Graphics2D::DrawLine(float x1, float y1, float x2, float y2)
{
	DrawLine(Fvec2(x1, y1), Fvec2(x2, y2));
}

void Graphics2D::DrawLine(Fvec2 start, Fvec2 end)
{
	if (start == end) return;
	Fmat4 pos = Translate((start + end) / 2.0f);
	Fvec2 dv = end - start;
	Fmat4 scale = Scale(Fvec2(dv.Norm(), line_width));
	Fmat4 rot = Rotate(std::atan2(dv[1], dv[0]));
	shader->SetUniform("u_model", pos * rot * scale);
	square_vao->Draw();
}

void Graphics2D::DrawText(std::string const& str, float x, float y)
{
	shader->SetUniform("u_draw_text", 1);
	Fvec2 text_size(0.0f);
	for (int i = 0u; i < str.size(); i++) {
		Glyph glyph = (*font)[str[i]];
		text_size[0] += glyph.advance >> 6;
		if (glyph.bearing[1] > text_size[1])
			text_size = glyph.bearing[1];
	}

	float char_x = 0.0f;
	for (int i = 0u; i < str.size(); i++)
	{
		Glyph glyph = (*font)[str[i]];
		float xpos = char_x + glyph.bearing[0];
		float ypos = (float)(glyph.bearing[1] - glyph.size[1]);
		float w = (float)glyph.size[0];
		float h = (float)glyph.size[1];

		Fmat4 model = Translate(Fvec2(xpos + w / 2.0f, ypos + h / 2.0f - text_size[1] / 2.0f)) * Scale(Fvec2(w, h));
		if (align == AlignCenter) model = Translate(Fvec2(-text_size[0] / 2.0f, 0.0f)) * model;
		else if (align == AlignRight) model = Translate(Fvec2(-text_size[0], 0.0f)) * model;
		model = Translate(Fvec2(x, y)) * Rotate(rotation) * model;

		glyph.texture->Bind(0);
		shader->SetUniform("u_model", model);
		square_vao->Draw();
		char_x += glyph.advance >> 6;
	}
	shader->SetUniform("u_draw_text", 0);
}

}