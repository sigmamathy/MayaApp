#include <Maya/deviceinfo.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Maya {

static std::vector<GLFWmonitor*> glfw_monitors;

static void InitGLFWMonitors()
{
	static bool first_call = false;
	if (first_call) return;
	int count;
	GLFWmonitor** ms = glfwGetMonitors(&count);
	glfw_monitors.reserve(count);
	for (int i = 0; i < count; i++)
		glfw_monitors.push_back(ms[i]);
	first_call = true;
}

int GetMonitorsCount()
{
	InitGLFWMonitors();
	return glfw_monitors.size();
}

MonitorInfo GetMonitorInfo(int number)
{
	InitGLFWMonitors();
	GLFWmonitor* monitor = glfw_monitors[number - 1];
	auto* vid = glfwGetVideoMode(monitor);
	return MonitorInfo {
		Ivec2(vid->width, vid->height),
		glfwGetMonitorName(monitor),
		monitor
	};
}

}