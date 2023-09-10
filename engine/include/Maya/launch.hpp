#pragma once

#include "./math.hpp"

namespace Maya {

constexpr int Vsync = 0;
constexpr int UnlimitedFPS = -1;

struct WindowConfiguration
{
	WindowConfiguration();
	Ivec2 size;
	std::string title;
	int fullscreen;
	int fps;
	int msaa;
	bool resizable,
		 decorated,
		 auto_iconify,
		 always_on_top,
		 maximized;
};

void CreateWindowInstance(WindowConfiguration& cfg);

bool InitializeApplication();
void CloseApplication();

}