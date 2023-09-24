#pragma once

#include "./model.hpp"
#include "./camera.hpp"

namespace Maya {

class Graphics3D
{
public:
	static void InitResources();
	static void FreeResources();

public:
	Graphics3D();
	void SetCamera(Camera3D const& camera);

	void Draw(Model3D const& model);
	void DrawCube();
};

}