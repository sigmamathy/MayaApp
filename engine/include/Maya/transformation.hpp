#pragma once

#include "./math.hpp"

namespace Maya {

// Translation matrix (2D)
Fmat4 Translate(Fvec2 position);

// Translation matrix
Fmat4 Translate(Fvec3 position);

// Rotation matrix (rotate about an axis)
Fmat4 Rotate(float rad, Fvec3 axis = Fvec3(0.0f, 0.0f, -1.0f));

// Scale matrix (2D)
Fmat4 Scale(Fvec2 scale);

// Scale matrix
Fmat4 Scale(Fvec3 scale);

// 3 dimensional view matrix, look at a certain position
Fmat4 LookAt(Fvec3 position, Fvec3 orientation, Fvec3 up = Fvec3(0.0f, 1.0f, 0.0f));

// Orthogonal projection matrix
Fmat4 OrthogonalProjection(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f);

// Perspective projection matrix
Fmat4 PerspectiveProjection(float fovy, float aspect, float near, float far);

}