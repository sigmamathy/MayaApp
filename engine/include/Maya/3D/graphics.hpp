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
	void SetViewMatrix(Fmat4 const& view);

	void Draw(Model3D const& model);
	void DrawCube();
};

}