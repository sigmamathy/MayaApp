#include <Maya.hpp>
#include <Maya/2D/graphics.hpp>
#include <Maya/3D/graphics.hpp>

class MyScene : public Maya::Scene
{
public:
	// Maya::Model3D::AsyncLoader load;
	Maya::Model3D* model;
	Maya::Camera3D camera;

	MyScene() {
		// load.AddTarget(model, "model.obj");
		// load.StartLoading();
		model = new Maya::Model3D("engine/resource/obj/backpack.obj");
	}

	~MyScene() {
		delete model;
	}

	void WhenUpdated(float elapsed) override
	{
		// load.LoadMeshes();

		// if (load.Ready(model))

		Maya::Graphics3D g;
		camera.InvokeDefaultControlUpdateCallback(elapsed);
		g.SetViewMatrix(camera.GetViewMatrix());

		g.Draw(*model);
		// g.DrawCube();
	}
};

MyScene* myscene;

static void AppEntry()
{
	Maya::WindowParameters param;
	param.size = { 1600, 900 };
	param.fps = 144;
	Maya::GameWindow::CreateInstance(param);

	myscene = new MyScene;
	ActivateScene(myscene);
}

static void AppExit()
{
	delete myscene;
}

MAYA_MAIN_FUNCTION(AppEntry, AppExit)