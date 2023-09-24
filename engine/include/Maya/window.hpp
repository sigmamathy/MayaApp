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

	void SetPosition(Ivec2 position);
	void SetSize(Ivec2 size);
	void SetTitle(std::string title);
	void SetFPS(int fps);

	Ivec2 GetPosition() const;
	Ivec2 GetSize() const;
	std::string GetTitle() const;
	int GetFPS() const;

	bool IsKeyPressed(KeyCode keycode) const;
	bool IsMouseButtonPressed(MouseButton button) const;

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
	MAYA_NO_COPY_CONSTRUCT(GameWindow);
	GameWindow() = default;
	void CreateWindowEventCallback();
};

}