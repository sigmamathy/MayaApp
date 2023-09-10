#include <Maya.hpp>
#include <Maya2D.hpp>
#include <Maya3D.hpp>

class MyScene : public Maya::Scene
{
public:

	float x = 0.0f;

	MyScene() {
	}

	void OnTick(float elapsed)
	{
		x += elapsed;
		Maya::Graphics2D g2d;
		g2d.SetRotation(x);
		g2d.SetGlowDirection(Maya::Graphics2D::GlowVertical);
		g2d.SetTexture("Maya");
		g2d.DrawOval(0, 0, 200, 200);

		//Maya::Graphics3D g3d;
		//g3d.DrawCube(elapsed);
	}

	void OnEvent(Maya::Event const& e)
	{
		
	}
};

bool Maya::ConfigureWindow(WindowConfiguration& cfg)
{
	cfg.size = { 1600, 900 };
	cfg.fps = Maya::Vsync;
	cfg.msaa = 4;
	return true;
}


bool Maya::InitResources()
{
	Graphics2D::InitResources();
	Graphics3D::InitResources();

	Assign("Maya", new Texture("engine/res/Maya.jpg"));

	Assign("Pursuit", new AudioStream("engine/res/Pursuit 2001.wav"));

	CreateScene<MyScene>("scene");
	SelectScene("scene");
	return true;
}