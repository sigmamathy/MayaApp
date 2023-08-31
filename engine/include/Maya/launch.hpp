#pragma once

#include "./math.hpp"

namespace Maya {

constexpr int Vsync = 0;
constexpr int UnlimitedFPS = -1;

struct WindowConfiguration
{
	Ivec2 size;
	std::string title;
	int fullscreen;
	int fps;
	bool resizable,
		 decorated,
		 auto_iconify,
		 always_on_top,
		 maximized;
private:
	WindowConfiguration();
	friend class PrivateControl;
};

void CloseApplication();

bool ConfigureWindow(WindowConfiguration& cfg);

bool InitResources();

}