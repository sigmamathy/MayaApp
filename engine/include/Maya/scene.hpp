#pragma once

#include "./core.hpp"
#include "./event.hpp"

namespace Maya {

class Scene
{
public:
	static std::unordered_map<std::string, Scene*>& GetScenes();
	static std::vector<Scene*> const& GetSelectedScenes();
	static void BeginScene(std::string const& name);
	static void EndScene(std::string const& name);
	static bool IsSceneSelected(std::string const& name);

public:
	virtual ~Scene() = default;
	virtual void WhenBegin() {}
	virtual void WhenEnd() {}
	virtual void WhenUpdated(float elapsed) {}
	virtual void WhenEventHappened(Event const& e) {}

private:
	static std::unordered_map<std::string, Scene*> scenes;
	static std::vector<Scene*> selected_scenes;
};

}