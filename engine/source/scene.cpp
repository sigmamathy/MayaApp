#include <Maya/scene.hpp>

namespace Maya {

static std::vector<Scene*> active_scenes;

std::vector<Scene*> const& GetActiveScenes()
{
	return active_scenes;
}

void ActivateScene(Scene* scene)
{
#if MAYA_DEBUG
	if (std::find(active_scenes.begin(), active_scenes.end(), scene) != active_scenes.end()) {
		MAYA_LOG("Warning (ActivateScene): the scene have already been activated.");
		return;
	}
#endif
	active_scenes.push_back(scene);
	scene->WhenBegin();
}

void DeactivateScene(Scene* scene)
{
#if MAYA_DEBUG
	if (std::find(active_scenes.begin(), active_scenes.end(), scene) == active_scenes.end()) {
		MAYA_LOG("Warning (DeactivateScene): the scene have not been activated yet.");
		return;
	}
#endif
	active_scenes.erase(std::remove(active_scenes.begin(), active_scenes.end(), scene), active_scenes.end());
	scene->WhenEnd();
}

}