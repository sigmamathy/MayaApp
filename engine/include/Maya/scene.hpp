#pragma once

#include "./core.hpp"
#include "./event.hpp"

namespace Maya {

class Scene
{
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

std::vector<Scene*> const& GetActiveScenes();
void ActivateScene(Scene* scene);
void DeactivateScene(Scene* scene);

}