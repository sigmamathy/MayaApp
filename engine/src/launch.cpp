#include <Maya/launch.hpp>
#include <Maya/window.hpp>
#include <Maya/scene.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <portaudio.h>
#include <thread>
#include <mutex>

namespace Maya {

static bool application_active = true;

static void InitLibraries()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Pa_Initialize();
}

static void TerminateLibraries()
{
	Pa_Terminate();
	glfwTerminate();
}

void CloseApplication()
{
	application_active = false;
}

}

static std::mutex scene_call_mutex;

static void update_loop()
{
	using namespace Maya;

	float begin = glfwGetTime();
	GameWindow& window = GameWindow::GetInstance();

	while (application_active)
	{
		float elapsed = glfwGetTime() - begin;
		if (elapsed < 1.0f / window.GetFPS()) continue;
		begin = glfwGetTime();

		std::unique_lock locker(scene_call_mutex);
		for (auto scene : Scene::GetSelectedScenes())
			scene->WhenUpdated(elapsed);
		locker.unlock();
	}
}

int main(int argc, char** argv)
{
	using namespace Maya;
	InitLibraries();

	AppEntryPoint();
	if (!GameWindow::IsInstanceCreated()) {
		TerminateLibraries();
		return 0;
	}

	float begin = glfwGetTime();
	GameWindow& window = GameWindow::GetInstance();

	std::thread update_loop_thread(update_loop);

	while (application_active)
	{
		float elapsed = glfwGetTime() - begin;
		if (elapsed < 1.0f / window.GetFPS()) continue;
		begin = glfwGetTime();

		std::unique_lock locker(scene_call_mutex);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		for (auto scene : Scene::GetSelectedScenes())
			scene->DrawGraphics();
		window.SwapBuffers();
		locker.unlock();

		glfwPollEvents();
	}

	update_loop_thread.join();
	for (auto& [name, scene] : Scene::GetScenes()) delete scene;
	delete &window;
	TerminateLibraries();
	return 0;
}