#pragma once

#include "../math.hpp"
#include "../shader.hpp"
#include "../font.hpp"

namespace Maya {

class Graphics2D
{
public:
	enum TextAlignment {
		AlignLeft,
		AlignCenter,
		AlignRight
	};

	enum GlowDirection {
		NoGlow,
		GlowHorizontal,
		GlowVertical,
		GlowDiagonal,
		GlowQuarterCircle,
		GlowQuarterCircleExclusive,
		GlowCenter,
		GlowCenterExclusive
	};

	static void InitResources();

	Graphics2D();

	void SetProjection(float width, float height);
	void SetProjection(Fvec2 size);

	void SetCameraPosition(Fvec2 position);
	void SetCameraPosition(float x, float y);
	void SetCameraZoom(Fvec2 zoom);
	void SetCameraZoom(float x, float y);

	void SetColor(unsigned int hex, float opacity = 1.0f);
	void SetTexture(std::string const& name);
	void SetTexture(Texture* texture);
	void SetGlowDirection(GlowDirection dir);

	void SetRotation(float radian);
	void SetOvalGood(unsigned int measure);
	void SetLineWidth(float width);
	void SetFont(std::string const& name);
	void SetFont(Font* font);
	void SetTextAlignment(TextAlignment align);

	void DrawRect(float x, float y, float width, float height);
	void DrawRect(Fvec2 position, Fvec2 scale);
	void DrawOval(float x, float y, float width, float height);
	void DrawOval(Fvec2 position, Fvec2 scale);
	void DrawLine(float x1, float y1, float x2, float y2);
	void DrawLine(Fvec2 start, Fvec2 end);
	void DrawText(std::string const& str, float x, float y);

private:
	Shader& shader;
	float rotation;
	unsigned int oval_measure;
	float line_width;
	Font* font;
	TextAlignment align;
};

}