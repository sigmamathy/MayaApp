#pragma once

#include "./math.hpp"
#include "./event.hpp"

namespace Maya {

struct WindowParameters
{
	WindowParameters();
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

class GameWindow
{
public:
	static GameWindow& CreateInstance(WindowParameters const& param);
	static GameWindow& GetInstance();
	static bool IsInstanceCreated();

	void SwapBuffers();

	void SetWindowPosition(Ivec2 position);
	void SetWindowSize(Ivec2 size);
	void SetWindowTitle(std::string title);
	void SetFPS(int fps);

	Ivec2 GetWindowPosition() const;
	Ivec2 GetWindowSize() const;
	std::string GetWindowTitle() const;
	int GetFPS() const;

private:
	static GameWindow* instance;
	void* glfw_window;
	struct DataStruct {
		Ivec2 position;
		Ivec2 size;
		std::string title;
		std::function<void(Event const&)> callback;
		int fps;
	} data;

private:
	GameWindow() = default;
	GameWindow(GameWindow const&) = delete;
	GameWindow& operator=(GameWindow const&) = delete;
	void CreateWindowEventCallback();
};

}