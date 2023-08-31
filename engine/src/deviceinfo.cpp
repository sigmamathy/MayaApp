#include "./private_control.hpp"

namespace Maya {

int GetMonitorsCount()
{
	auto& ctrl = PrivateControl::Instance();
	return ctrl.monitors.size();
}

MonitorInfo GetMonitorInfo(int number)
{
	auto& ctrl = PrivateControl::Instance();
	GLFWmonitor* monitor = ctrl.monitors[number - 1];

	auto* vid = glfwGetVideoMode(monitor);
	return MonitorInfo{
		Ivec2(vid->width, vid->height),
		glfwGetMonitorName(monitor)
	};
}

}