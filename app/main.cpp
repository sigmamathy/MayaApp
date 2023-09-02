#include <Maya.hpp>
#include <Maya2D.hpp>

class MyScene : public Maya::Scene
{
public:
	void OnTick(float elapsed)
	{
		Maya::Graphics2D g2d;

		// g2d.SetTexture("Maya");
		g2d.SetColor(0x00FF00);
		g2d.DrawLine(-100, -100, 50, 100);
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
	Graphics2D::InitResources();

	Assign("Maya", new Texture("engine/res/Maya.jpg"));

	AssignScene<MyScene>("scene");
	SelectScene("scene");
	return true;
}