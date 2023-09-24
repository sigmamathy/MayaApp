#include <Maya.hpp>
#include <Maya/2D/graphics.hpp>
#include <Maya/3D/graphics.hpp>

class MyScene : public Maya::Scene
{
public:
	Maya::Model3D* model;

	MyScene() {
		model = new Maya::Model3D("engine/resource/obj/backpack.obj");
	}

	~MyScene() {
		delete model;
	}

	void WhenUpdated(float elapsed) override
	{
		Maya::Graphics3D g;

		g.Draw(*model);
		// g.DrawCube();
	}
};

void AppEntryPoint()
{
	Maya::WindowParameters param;
	param.size = { 1600, 900 };
	param.fps = 144;
	Maya::GameWindow::CreateInstance(param);

	Maya::Scene::GetScenes()["my scene"] = new MyScene;
	Maya::Scene::BeginScene("my scene");
}

MAYA_MAIN_FUNCTION(AppEntryPoint)