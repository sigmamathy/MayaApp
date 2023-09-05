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

	void OnBegin()
	{
	}

	void OnTick(float elapsed)
	{
		Maya::Graphics2D g2d;
		velocity[1] -= elapsed * 981.0f;
		position += elapsed * velocity;

		//g2d.DrawRect(position, Maya::Fvec2(50));

		g2d.SetTextAlignment(Maya::Graphics2D::AlignCenter);
		float t = Maya::GetAudioStream("Pursuit").GetAudioStreamStatus().time;
		g2d.DrawText("Pursuit: " + std::to_string(t), 0, 0);
	}

	void OnEvent(Maya::Event const& e)
	{
		if (auto* ke = Maya::EventCast<Maya::KeyEvent>(e)) {
			if (ke->down) {
				if (ke->keycode == Maya::KeyW)
					velocity[1] = 600.0f;
				if (ke->keycode == Maya::KeySpace)
					Maya::GetAudioStream("Pursuit").Start();
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

	Assign("Pursuit", new AudioStream("engine/res/Pursuit 2001.wav"));

	CreateScene<MyScene>("scene");
	SelectScene("scene");
	return true;
}