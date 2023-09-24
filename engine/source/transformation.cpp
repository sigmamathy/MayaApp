#include <Maya/transformation.hpp>

namespace Maya {

Fmat4 Translate(Fvec2 position)
{
	return Translate(Fvec3(position[0], position[1], 0.0f));
}

Fmat4 Translate(Fvec3 position)
{
	return Fmat4(
		1.0f, 0.0f, 0.0f, position[0],
		0.0f, 1.0f, 0.0f, position[1],
		0.0f, 0.0f, 1.0f, position[2],
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Fmat4 Rotate(float rad, Fvec3 axis)
{
	axis = Normalize(axis);
	float c = std::cos(rad), s = std::sin(rad);
	auto fun1 = [&](int i) -> float { return c + axis[i] * axis[i] * (1 - c); };
	auto fun2 = [&](int i, int j, int k) -> float { return (1 - c) * axis[i] * axis[j] + s * axis[k]; };
	auto fun3 = [&](int i, int j, int k) -> float { return (1 - c) * axis[i] * axis[j] - s * axis[k]; };
	return Fmat4(
		fun1(0), fun3(0, 1, 2), fun2(0, 2, 1), 0.0f,
		fun2(0, 1, 2), fun1(1), fun3(1, 2, 0), 0.0f,
		fun3(0, 2, 1), fun2(1, 2, 0), fun1(2), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Fmat4 Scale(Fvec2 scale)
{
	return Scale(Fvec3(scale[0], scale[1], 1.0f));
}

Fmat4 Scale(Fvec3 scale)
{
	return Fmat4(
		scale[0], 0.0f, 0.0f, 0.0f,
		0.0f, scale[1], 0.0f, 0.0f,
		0.0f, 0.0f, scale[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Fmat4 LookAt(Fvec3 position, Fvec3 orientation, Fvec3 up)
{
	Fmat4 res(1.0f);
	Fvec3 f = Normalize(orientation);
	Fvec3 s = Normalize(Cross(f, up));
	Fvec3 u = Cross(s, f);
	res.Get(0, 0) = s[0];
	res.Get(0, 1) = s[1];
	res.Get(0, 2) = s[2];
	res.Get(1, 0) = u[0];
	res.Get(1, 1) = u[1];
	res.Get(1, 2) = u[2];
	res.Get(2, 0) = -f[0];
	res.Get(2, 1) = -f[1];
	res.Get(2, 2) = -f[2];
	res.Get(0, 3) = -Dot(s, position);
	res.Get(1, 3) = -Dot(u, position);
	res.Get(2, 3) = Dot(f, position);
	return res;
}

Fmat4 OrthogonalProjection(float left, float right, float bottom, float top, float near, float far)
{
	float x = right - left,
		y = top - bottom,
		z = far - near;
	return Fmat4(
		2.0f / x, 0.0f, 0.0f, -(right + left) / x,
		0.0f, 2.0f / y, 0.0f, -(top + bottom) / y,
		0.0f, 0.0f, -2.0f / z, -(far + near) / z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Fmat4 OrthogonalProjection(Fvec2 size)
{
	return OrthogonalProjection(
		-size[0] / 2.0f, size[0] / 2.0f, -size[1] / 2.0f, size[1] / 2.0f);
}

Fmat4 PerspectiveProjection(float fovy, float aspect, float near, float far)
{
	Fmat4 res(0.0f);
	float const tanHalfFovy = tan(fovy / 2.0f);
	res.Get(0, 0) = 1.0f / (-aspect * tanHalfFovy);
	res.Get(1, 1) = 1.0f / (tanHalfFovy);
	res.Get(2, 2) = -(far + near) / (far - near);
	res.Get(3, 2) = -1.0f;
	res.Get(2, 3) = -(2.0f * far * near) / (far - near);
	return res;
}

}