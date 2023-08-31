#pragma once

#include "../math.hpp"
#include "../scene.hpp"
#include "../shader.hpp"

namespace Maya {

void LoadResources2D();

class Scene2D;

class Renderer2D
{
public:
	void UpdateViewAndProjection();
	void SetColor(unsigned int hex, float opacity = 1.0f);
	void SetRotation(float radian);
	void SetOvalGood(unsigned int measure);

	void DrawRect(int x, int y, int width, int height);
	void DrawRect(Fvec2 position, Fvec2 scale);
	void DrawOval(int x, int y, int width, int height);
	void DrawOval(Fvec2 position, Fvec2 scale);
	//void DrawTexture(std::string const& name, int x, int y, int width, int height);
	//void DrawTexture(std::string const& name, Fvec2 position, Fvec2 scale);

private:
	Scene2D* parent;
	float rotation;
	unsigned int oval_measure;

private:
	Renderer2D(Scene2D* parent);
	friend class Scene2D;
};

class Scene2D : public Scene
{
protected:
	Scene2D();
	void SetProjection(Fvec2 size);
	void SetCameraPosition(Fvec2 position);
	void SetCameraZoom(Fvec2 zoom);
	Renderer2D CreateRenderer();

private:
	Fmat4 projection;
	Fmat4 camera_pos, camera_scale;
	friend class Renderer2D;
};

}