#pragma once

#include <Maya.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>

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
	} windata;

	std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
	std::unordered_map<std::string, Shader*> shaders;
	std::unordered_map<std::string, VertexArray*> vaos;

	Scene* current_scene = nullptr;
	Shader* current_shader = nullptr;
	VertexArray* current_vao = nullptr;

public:
	int MainFunction();
	Shader& AssignShader_Temp(std::string const& name, std::string const& vertex, std::string const& fragment);
	VertexArray& AssignVAO_Temp(std::string const& name, int count, Primitives primitive);
	void ShaderDrawVAO(VertexArray& vao);
};


}