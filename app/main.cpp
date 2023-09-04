#include <Maya.hpp>
#include <Maya2D.hpp>

class MyScene : public Maya::Scene
{
public:
	Maya::Fvec2 position, velocity;

	MyScene() {
		position = { 0, 0 };
		velocity = { 0, 0 };
	}

	void OnTick(float elapsed)
	{
		Maya::Graphics2D g2d;
		velocity[1] -= elapsed * 981.0f;
		position += elapsed * velocity;

		g2d.DrawRect(position, Maya::Fvec2(50));
	}

	void OnEvent(Maya::Event const& e)
	{
		if (auto* ke = Maya::EventCast<Maya::KeyEvent>(e)) {
			if (ke->down) {
				if (ke->keycode == Maya::KeyW)
					velocity[1] = 600.0f;
			}
		}
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