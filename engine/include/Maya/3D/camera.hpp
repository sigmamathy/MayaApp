#pragma once

#include "../math.hpp"
#include "../event.hpp"

namespace Maya {

class Camera3D
{
public:
	Camera3D();
	Fmat4 GetViewMatrix() const;
	void InvokeDefaultControlUpdateCallback(float elapsed);

	void SetPosition(Fvec3 position);
	void SetHorizontalAngle(float angle);
	void SetVerticalAngle(float angle);

	Fvec3 GetPosition() const;
	float GetHorizontalAngle() const;
	float GetVerticalAngle() const;

private:
	Fvec3 position;
	float horizontal_angle, vertical_angle;
};

}