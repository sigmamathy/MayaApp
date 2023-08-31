#include <Maya.hpp>
#include <Maya2D.hpp>

class MyScene : public Maya::Scene2D
{
public:
	float rotate;

	MyScene()
	{
		rotate = 0.0f;
	}

	void OnTick(float elapsed)
	{
		Maya::Renderer2D r = CreateRenderer();
		r.SetColor(0x123456);

		rotate += elapsed;
		r.SetRotation(rotate);

		r.DrawOval(0, 0, 100, 50);
	}
};

bool Maya::ConfigureWindow(WindowConfiguration& cfg)
{
	cfg.size = { 1600, 900 };
	cfg.fps = Maya::Vsync;
	return true;
}


bool Maya::InitResources()
{
	LoadResources2D();
	
	AssignScene<MyScene>("scene");
	SelectScene("scene");
	return true;
}