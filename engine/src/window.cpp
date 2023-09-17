#include <Maya/window.hpp>
#include <Maya/launch.hpp>
#include <Maya/deviceinfo.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Maya {

GameWindow* GameWindow::instance = nullptr;

WindowParameters::WindowParameters()
	: size(1280, 720), title("Maya Application"), fullscreen(0), fps(144), msaa(1),
	  resizable(true), decorated(true), auto_iconify(true), always_on_top(false), maximized(false)
{
}

GameWindow& GameWindow::CreateInstance(WindowParameters const& param)
{
	glfwWindowHint(GLFW_RESIZABLE, param.resizable);
	glfwWindowHint(GLFW_DECORATED, param.decorated);
	glfwWindowHint(GLFW_AUTO_ICONIFY, param.auto_iconify);
	glfwWindowHint(GLFW_FLOATING, param.always_on_top);
	glfwWindowHint(GLFW_MAXIMIZED, param.maximized);
	glfwWindowHint(GLFW_SAMPLES, param.msaa);

	GLFWwindow* previous_window = nullptr;
	if (instance) previous_window = (GLFWwindow*) instance->glfw_window;
	else instance = new GameWindow;

	GLFWmonitor* monitor = param.fullscreen > 0 ? (GLFWmonitor*) GetMonitorInfo(param.fullscreen - 1).glfw_monitor : nullptr;
	auto& data = instance->data;
	data.size = param.fullscreen > 0 ? GetMonitorInfo(param.fullscreen).resolution : param.size;
	data.title = param.title;
	data.callback = [](auto&) {};
	data.fps = param.fps;

	GLFWwindow* window = glfwCreateWindow(data.size[0], data.size[1], data.title.c_str(), monitor, previous_window);
	instance->glfw_window = window;
	if (previous_window) glfwDestroyWindow(previous_window);

	glfwMakeContextCurrent(window);
	glfwGetWindowPos(window, &data.position[0], &data.position[1]);
	glfwSetWindowUserPointer(window, &data);
	instance->CreateWindowEventCallback();
	gladLoadGL();
	glViewport(0, 0, data.size[0], data.size[1]);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return *instance;
}

GameWindow& GameWindow::GetInstance()
{
	return *instance;
}

bool GameWindow::IsInstanceCreated()
{
	return instance;
}

void GameWindow::CreateWindowEventCallback()
{
	GLFWwindow* window = (GLFWwindow*) glfw_window;

	glfwSetKeyCallback(window,
	[](GLFWwindow* window, int key, int scancode, int act, int modes) {
		auto& data = *(DataStruct*)glfwGetWindowUserPointer(window);
		if (act != GLFW_REPEAT)
			data.callback(KeyEvent(KeyCode(key), act == GLFW_PRESS));
	});

	glfwSetMouseButtonCallback(window,
	[](GLFWwindow* window, int button, int act, int modes) {
		auto& data = *(DataStruct*)glfwGetWindowUserPointer(window);
		if (act != GLFW_REPEAT)
			data.callback(MouseEvent(MouseButton(button), act == GLFW_PRESS));
	});

	glfwSetCursorPosCallback(window,
	[](GLFWwindow* window, double x, double y) {
		auto& data = *(DataStruct*)glfwGetWindowUserPointer(window);
		Ivec2 res(x, y);
		data.callback(MouseMovedEvent(res));
	});

	glfwSetScrollCallback(window,
	[](GLFWwindow* window, double x, double y) {
		auto& data = *(DataStruct*)glfwGetWindowUserPointer(window);
		Ivec2 res(x, y);
		data.callback(MouseScrolledEvent(res));
	});

	glfwSetWindowFocusCallback(window,
	[](GLFWwindow* window, int focus) {
		auto& data = *(DataStruct*)glfwGetWindowUserPointer(window);
		data.callback(WindowFocusedEvent(focus));
	});

	glfwSetWindowCloseCallback(window,
	[](GLFWwindow* window) {
		auto& data = *(DataStruct*)glfwGetWindowUserPointer(window);
		data.callback(WindowClosedEvent());
		CloseApplication();
	});

	glfwSetWindowSizeCallback(window,
	[](GLFWwindow* window, int width, int height) {
		auto& data = *(DataStruct*)glfwGetWindowUserPointer(window);
		data.size = Ivec2(width, height);
		data.callback(WindowResizedEvent(data.size));
	});

	glfwSetWindowPosCallback(window,
	[](GLFWwindow* window, int x, int y) {
		auto& data = *(DataStruct*)glfwGetWindowUserPointer(window);
		data.position = Ivec2(x, y);
		data.callback(WindowMovedEvent(data.position));
	});
}

void GameWindow::SwapBuffers()
{
	glfwSwapBuffers((GLFWwindow*)glfw_window);
}

void GameWindow::SetWindowPosition(Ivec2 position)
{
	data.position = position;
	glfwSetWindowPos((GLFWwindow*)glfw_window, position[0], position[1]);
}

void GameWindow::SetWindowSize(Ivec2 size)
{
	data.size = size;
	glfwSetWindowSize((GLFWwindow*)glfw_window, size[0], size[1]);
}

void GameWindow::SetWindowTitle(std::string title)
{
	data.title = title;
	glfwSetWindowTitle((GLFWwindow*)glfw_window, title.c_str());
}

void GameWindow::SetFPS(int fps)
{
	data.fps = fps;
}

Ivec2 GameWindow::GetWindowPosition() const
{
	return data.position;
}

Ivec2 GameWindow::GetWindowSize() const
{
	return data.size;
}

std::string GameWindow::GetWindowTitle() const
{
	return data.title;
}

int GameWindow::GetFPS() const
{
	return data.fps;
}

}