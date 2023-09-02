#pragma once

#include "../math.hpp"
#include "../scene.hpp"
#include "../shader.hpp"

namespace Maya {

class Graphics2D
{
public:
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
	void SetRotation(float radian);
	void SetOvalGood(unsigned int measure);

	void DrawRect(float x, float y, float width, float height);
	void DrawRect(Fvec2 position, Fvec2 scale);
	void DrawOval(float x, float y, float width, float height);
	void DrawOval(Fvec2 position, Fvec2 scale);
	void DrawLine(float x1, float y1, float x2, float y2);
	void DrawLine(Fvec2 start, Fvec2 end);

private:
	Shader& shader;
	float rotation;
	unsigned int oval_measure;
};

}