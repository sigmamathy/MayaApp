#include <Maya.hpp>
#include <Maya/2D/graphics.hpp>

class MyScene : public Maya::Scene
{
public:
	float y = 0.0f;

	void WhenUpdated(float elapsed) override
	{
		y += elapsed * 50.0f;
		Maya::Graphics2D g2d;
		g2d.DrawRect(0, y, 100, 100);
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