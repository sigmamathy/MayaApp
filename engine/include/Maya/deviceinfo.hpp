#pragma once

#include "./math.hpp"

namespace Maya {

constexpr int PrimaryMonitor = 1;

struct MonitorInfo
{
	Ivec2 resolution;
	std::string name;
};

int GetMonitorsCount();

MonitorInfo GetMonitorInfo(int number);

}