#include <Maya/launch.hpp>
#include <Maya/window.hpp>
#include <Maya/scene.hpp>
#include <Maya/2D/graphics.hpp>
#include <Maya/3D/graphics.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <portaudio.h>

namespace Maya {

static bool application_active = true;

void CloseApplication()
{
	application_active = false;
}

struct RAII_ExceptionSafeDelete
{
	std::function<void()> exitfunc;
	~RAII_ExceptionSafeDelete()
	{
		exitfunc();
		Graphics2D::FreeResources();
		Graphics3D::FreeResources();
		for (auto& [name, scene] : Scene::GetScenes()) delete scene;
		if (GameWindow::IsInstanceCreated())
			delete& GameWindow::GetInstance();
		Pa_Terminate();
		glfwTerminate();
	}
};


int internal::MainFunction(std::function<void()> const& entryfunc, std::function<void()> const& exitfunc)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Pa_Initialize();

	RAII_ExceptionSafeDelete _;
	_.exitfunc = exitfunc;
	entryfunc();

	if (!GameWindow::IsInstanceCreated())
		return 0;

	GameWindow& window = GameWindow::GetInstance();
	float min_time_delay = 1.0f / window.GetFPS();
	float begin = glfwGetTime();
	float pe_elapsed = 0.0f;

	while (application_active)
	{
		float elapsed = glfwGetTime() - begin - pe_elapsed;
		if (elapsed < min_time_delay) continue;
		begin = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		for (auto scene : Scene::GetSelectedScenes())
			scene->WhenUpdated(elapsed);
		window.SwapBuffers();

		float pe_begin = glfwGetTime();
		glfwPollEvents();
		pe_elapsed = glfwGetTime() - pe_begin;
	}

	return 0;
}

}