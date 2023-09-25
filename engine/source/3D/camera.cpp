#include <Maya/3D/camera.hpp>
#include <Maya/transformation.hpp>
#include <Maya/window.hpp>

namespace Maya {

Camera3D::Camera3D()
	: position(0), horizontal_angle(0), vertical_angle(0)
{
}

Fmat4 Camera3D::GetViewMatrix() const
{
	Fvec4 orientation(0.0f, 0.0f, 1.0f, 1.0f);
	orientation = Rotate(vertical_angle, Fvec3(-1.0f, 0.0f, 0.0f)) * orientation;
	orientation = Rotate(horizontal_angle, Fvec3(0.0f, 1.0f, 0.0f)) * orientation;
	return LookAt(position, orientation.Get(0, 1, 2));
}

void Camera3D::InvokeDefaultControlUpdateCallback(float elapsed)
{
	constexpr float half_pi = 3.1415926536f / 2;
	constexpr float speed = 5.0f;
	constexpr float sensitivity = 2.0f;

	auto& window = GameWindow::GetInstance();
	Fvec3 ori(std::sinf(horizontal_angle), 0.0f, std::cosf(horizontal_angle));
	constexpr Fvec3 up(0.0f, 1.0f, 0.0f);

	if (window.IsKeyPressed(KeyW)) position += elapsed * speed * ori;
	if (window.IsKeyPressed(KeyS)) position -= elapsed * speed * ori;

	if (window.IsKeyPressed(KeyA)) position += elapsed * speed * Normalize(Cross(ori, up));
	if (window.IsKeyPressed(KeyD)) position -= elapsed * speed * Normalize(Cross(ori, up));

	if (window.IsKeyPressed(KeyE)) position[1] += elapsed * speed;
	if (window.IsKeyPressed(KeyQ)) position[1] -= elapsed * speed;

	if (window.IsKeyPressed(KeyRight)) horizontal_angle += elapsed * sensitivity;
	if (window.IsKeyPressed(KeyLeft)) horizontal_angle -= elapsed * sensitivity;

	if (window.IsKeyPressed(KeyUp) && vertical_angle < half_pi - 0.1f) vertical_angle += elapsed * sensitivity;
	if (window.IsKeyPressed(KeyDown) && vertical_angle > 0.1f - half_pi) vertical_angle -= elapsed * sensitivity;
}

void Camera3D::SetPosition(Fvec3 position)
{
	this->position = position;
}

void Camera3D::SetHorizontalAngle(float angle)
{
	horizontal_angle = angle;
}

void Camera3D::SetVerticalAngle(float angle)
{
	vertical_angle = angle;
}

Fvec3 Camera3D::GetPosition() const
{
	return position;
}

float Camera3D::GetHorizontalAngle() const
{
	return horizontal_angle;
}

float Camera3D::GetVerticalAngle() const
{
	return vertical_angle;
}

}