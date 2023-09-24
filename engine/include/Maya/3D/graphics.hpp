#pragma once

#include "./model.hpp"

namespace Maya {

class Graphics3D
{
public:
	static void InitResources();
	static void FreeResources();

public:
	Graphics3D();

	void Draw(Model3D const& model);
	void DrawCube();
};

}