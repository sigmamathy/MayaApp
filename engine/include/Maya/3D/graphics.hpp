#pragma once

#include "../shader.hpp"

namespace Maya {

class Graphics3D
{
public:
	static constexpr Fvec3 right	= { 1, 0, 0 };
	static constexpr Fvec3 up		= { 0, 1, 0 };
	static constexpr Fvec3 front	= { 0, 0, 1 };

	static void InitResources();

	Graphics3D();
	void DrawCube(float elapsed);

private:
	Shader& shader;
};

}