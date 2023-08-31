#include "./private_control.hpp"

namespace Maya {

static void CreateWindowEventCallback(GLFWwindow* window)
{
	using WindowData = PrivateControl::WindowData;

	glfwSetKeyCallback(window,
	[](GLFWwindow* window, int key, int scancode, int act, int modes) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		if (act != GLFW_REPEAT)
			data.callback(KeyEvent(KeyCode(key), act == GLFW_PRESS));
	});

	glfwSetMouseButtonCallback(window,
	[](GLFWwindow* window, int button, int act, int modes) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		if (act != GLFW_REPEAT)
			data.callback(MouseEvent(MouseButton(button), act == GLFW_PRESS));
	});

	glfwSetCursorPosCallback(window,
	[](GLFWwindow* window, double x, double y) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		Ivec2 res(x, y);
		data.callback(MouseMovedEvent(res));
	});

	glfwSetScrollCallback(window,
	[](GLFWwindow* window, double x, double y) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		Ivec2 res(x, y);
		data.callback(MouseScrolledEvent(res));
	});

	glfwSetWindowFocusCallback(window,
	[](GLFWwindow* window, int focus) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.callback(WindowFocusedEvent(focus));
	});

	glfwSetWindowCloseCallback(window,
	[](GLFWwindow* window) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.callback(WindowClosedEvent());
	});

	glfwSetWindowSizeCallback(window,
	[](GLFWwindow* window, int width, int height) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.size = Ivec2(width, height);
		data.callback(WindowResizedEvent(data.size));
	});

	glfwSetWindowPosCallback(window,
	[](GLFWwindow* window, int x, int y) {
		auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.position = Ivec2(x, y);
		data.callback(WindowMovedEvent(data.position));
	});
}

int PrivateControl::MainFunction()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int count;
	GLFWmonitor** ms = glfwGetMonitors(&count);
	monitors.reserve(count);
	for (int i = 0; i < count; i++)
		monitors.push_back(ms[i]);

	WindowConfiguration config;
	if (!ConfigureWindow(config))
		return -1;

	glfwWindowHint(GLFW_RESIZABLE,		config.resizable);
	glfwWindowHint(GLFW_DECORATED,		config.decorated);
	glfwWindowHint(GLFW_AUTO_ICONIFY,	config.auto_iconify);
	glfwWindowHint(GLFW_FLOATING,		config.always_on_top);
	glfwWindowHint(GLFW_MAXIMIZED,		config.maximized);

	GLFWmonitor* monitor = config.fullscreen > 0 ? monitors[config.fullscreen - 1] : nullptr;
	windata.size = config.fullscreen > 0 ? GetMonitorInfo(config.fullscreen).resolution : config.size;
	windata.title = config.title;
	windata.callback = [](auto&) {};
	
	window = glfwCreateWindow(windata.size[0], windata.size[1], config.title.c_str(), monitor, 0);
	glfwMakeContextCurrent(window);
	glfwGetWindowPos(window, &windata.position[0], &windata.position[1]);
	glfwSetWindowUserPointer(window, &windata);
	CreateWindowEventCallback(window);
	gladLoadGL();
	glViewport(0, 0, windata.size[0], windata.size[1]);
	
	AssignScene<Scene>("");
	
	if (!InitResources())
		return -1;

	if (config.fps == Vsync)
		glfwSwapInterval(1);
	float begin = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float elapsed = glfwGetTime() - begin;
		if (config.fps > 0 && elapsed < 1.0f / config.fps) continue;
		begin = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		
		if (current_scene)
			current_scene->OnTick(elapsed);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	return 0;
}

PrivateControl::~PrivateControl()
{
	for (auto& pair : shaders) delete pair.second;
	for (auto& pair : vaos) delete pair.second;
	for (auto& pair : textures) delete pair.second;
	glfwTerminate();
}

WindowConfiguration::WindowConfiguration()
	: size(1280, 720), title("Maya Application"), fullscreen(0), fps(UnlimitedFPS),
	  resizable(true), decorated(true), auto_iconify(true), always_on_top(false), maximized(false)
{
}

void CloseApplication()
{
	auto& ctrl = PrivateControl::Instance();
	glfwSetWindowShouldClose(ctrl.window, true);
}

}

int main(int argc, char** argv)
{
	auto& ctrl = Maya::PrivateControl::Instance();
	return ctrl.MainFunction();
}