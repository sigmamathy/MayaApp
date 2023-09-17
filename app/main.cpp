#include <Maya.hpp>

class MyScene : public Maya::Scene
{
public:
	void WhenUpdated(float elapsed) {
		std::cout << "updated\n";
	}
	void DrawGraphics() {
		std::cout << "graphics\n";
	}
};

void Maya::AppEntryPoint()
{
	WindowParameters param;
	// param.fps = 500;
	GameWindow::CreateInstance(param);

	Scene::GetScenes()["my scene"] = new MyScene;
	Scene::BeginScene("my scene");
}