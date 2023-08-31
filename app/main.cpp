#include <Maya.hpp>

class MyScene : public Maya::Scene
{
public:
	void OnTick(float elapsed)
	{
		auto& shader = Maya::GetShader("Maya_2D");
		auto& vao = Maya::GetVAO("a");
		shader.Draw(vao);
	}

	void OnEvent(Maya::Event const& e)
	{
		
	}
};

bool Maya::ConfigureWindow(WindowConfiguration& cfg)
{
	// cfg.fullscreen = Maya::PrimaryMonitor;
	cfg.size = { 1600, 900 };
	cfg.fps = 60;
	return true;
}


bool Maya::InitResources()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};
	AssignShader("Maya_2D", "engine/res/Maya_2D.vert.glsl", "engine/res/Maya_2D.frag.glsl");
	AssignVAO("a", 3).LinkVBO(vertices, VertexLayout(3));

	AssignScene<MyScene>("scene");
	SelectScene("scene");
	return true;
}