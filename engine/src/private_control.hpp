#pragma once

#include <Maya.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <portaudio.h>

namespace Maya {

class PrivateControl
{
private:
	PrivateControl() {}
	~PrivateControl();
	PrivateControl(PrivateControl const&) = delete;
	PrivateControl& operator=(PrivateControl const&) = delete;

public:
	static PrivateControl& Instance() {
		static PrivateControl instance;
		return instance;
	}

public:
	GLFWwindow* window;
	std::vector<GLFWmonitor*> monitors;
	struct WindowData {
		Ivec2 position;
		Ivec2 size;
		std::string title;
		std::function<void(Event const&)> callback;
		int fps;
	} windata;

	std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
	Scene* current_scene = nullptr;

public:
	int MainFunction();
};


}