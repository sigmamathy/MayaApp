#pragma once

#include "./math.hpp"

namespace Maya {

namespace internal {
#define MAYA_MAIN_FUNCTION(...) int main(void) { return Maya::internal::MainFunction(__VA_ARGS__); }
	int MainFunction(std::function<void()> const& entryfunc, std::function<void()> const& exitfunc = []() {});
}

void CloseApplication();

}