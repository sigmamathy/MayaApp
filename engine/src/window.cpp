#include "./private_control.hpp"

namespace Maya {

void SetWindowPosition(Ivec2 position)
{
	auto& ctrl = PrivateControl::Instance();
	ctrl.windata.position = position;
	glfwSetWindowPos(ctrl.window, position[0], position[1]);
}

void SetWindowSize(Ivec2 size)
{
	auto& ctrl = PrivateControl::Instance();
	ctrl.windata.size = size;
	glfwSetWindowSize(ctrl.window, size[0], size[1]);
}

void SetWindowTitle(std::string title)
{
	auto& ctrl = PrivateControl::Instance();
	ctrl.windata.title = title;
	glfwSetWindowTitle(ctrl.window, title.c_str());
}

Ivec2 GetWindowPosition()
{
	auto& ctrl = PrivateControl::Instance();
	return ctrl.windata.position;
}

Ivec2 GetWindowSize()
{
	auto& ctrl = PrivateControl::Instance();
	return ctrl.windata.size;
}

std::string GetWindowTitle()
{
	auto& ctrl = PrivateControl::Instance();
	return ctrl.windata.title;
}

}