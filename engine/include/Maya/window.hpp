#pragma once

#include "./math.hpp"

namespace Maya {

void SetWindowPosition(Ivec2 position);

void SetWindowSize(Ivec2 size);

void SetWindowTitle(std::string title);

Ivec2 GetWindowPosition();

Ivec2 GetWindowSize();

std::string GetWindowTitle();

}