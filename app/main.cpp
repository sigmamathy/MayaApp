#include <Maya.hpp>
#include <Maya2D.hpp>

class MyScene : public Maya::Scene
{
public:

	void OnTick(float elapsed)
	{
		Maya::Graphics2D g2d;
		
		g2d.SetTexture("Maya");
		g2d.SetGlowDirection(Maya::Graphics2D::GlowCenter);
		g2d.DrawRect(0, 0, 300, 300);
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

	CreateScene<MyScene>("scene");
	SelectScene("scene");
	return true;
}