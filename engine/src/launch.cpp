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

void CreateWindowInstance(WindowConfiguration& cfg)
{
	auto& ctrl = PrivateControl::Instance();

	glfwWindowHint(GLFW_RESIZABLE,		cfg.resizable);
	glfwWindowHint(GLFW_DECORATED,		cfg.decorated);
	glfwWindowHint(GLFW_AUTO_ICONIFY,	cfg.auto_iconify);
	glfwWindowHint(GLFW_FLOATING,		cfg.always_on_top);
	glfwWindowHint(GLFW_MAXIMIZED,		cfg.maximized);
	glfwWindowHint(GLFW_SAMPLES,		cfg.msaa);

	GLFWmonitor* monitor = cfg.fullscreen > 0 ? ctrl.monitors[cfg.fullscreen - 1] : nullptr;
	ctrl.windata.size = cfg.fullscreen > 0 ? GetMonitorInfo(cfg.fullscreen).resolution : cfg.size;
	ctrl.windata.title = cfg.title;
	ctrl.windata.callback = [](auto&) {};
	ctrl.windata.fps = cfg.fps;

	ctrl.window = glfwCreateWindow(ctrl.windata.size[0], ctrl.windata.size[1], cfg.title.c_str(), monitor, 0);
	glfwMakeContextCurrent(ctrl.window);
	glfwGetWindowPos(ctrl.window, &ctrl.windata.position[0], &ctrl.windata.position[1]);
	glfwSetWindowUserPointer(ctrl.window, &ctrl.windata);
	if (cfg.fps == Vsync) glfwSwapInterval(1);
	CreateWindowEventCallback(ctrl.window);
	gladLoadGL();
	glViewport(0, 0, ctrl.windata.size[0], ctrl.windata.size[1]);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int PrivateControl::MainFunction()
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Pa_Initialize();

	int count;
	GLFWmonitor** ms = glfwGetMonitors(&count);
	monitors.reserve(count);
	for (int i = 0; i < count; i++)
		monitors.push_back(ms[i]);

	if (!InitializeApplication() || !window)
		return -1;

	float begin = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float elapsed = glfwGetTime() - begin;
		if (windata.fps > 0 && elapsed < 1.0f / windata.fps) continue;
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
	Pa_Terminate();
	glfwTerminate();
}

WindowConfiguration::WindowConfiguration()
	: size(1280, 720), title("Maya Application"), fullscreen(0), fps(UnlimitedFPS), msaa(1),
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