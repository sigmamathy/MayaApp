#include <Maya/scene.hpp>

namespace Maya {

std::unordered_map<std::string, Scene*> Scene::scenes;
std::vector<Scene*> Scene::selected_scenes;

std::unordered_map<std::string, Scene*>& Scene::GetScenes()
{
	return scenes;
}

std::vector<Scene*> const& Scene::GetSelectedScenes()
{
	return selected_scenes;
}

void Scene::BeginScene(std::string const& name)
{
#if MAYA_DEBUG
	if (!scenes.count(name)) {
		MAYA_LOG("Warning (Scene::BeginScene): the name \"" << name << "\" cannot be found in Scene::scenes");
		return;
	}
	if (std::find(selected_scenes.begin(), selected_scenes.end(), scenes.at(name)) != selected_scenes.end()) {
		MAYA_LOG("Warning (Scene::BeginScene): the name \"" << name << "\" is already in Scene::selected_scenes");
		return;
	}
#endif
	Scene* scene = scenes.at(name);
	selected_scenes.push_back(scene);
	scene->WhenBegin();
}

void Scene::EndScene(std::string const& name)
{
#if MAYA_DEBUG
	if (!scenes.count(name)) {
		MAYA_LOG("Warning (Scene::BeginScene): the name \"" << name << "\" cannot be found in Scene::scenes");
		return;
	}
	if (std::find(selected_scenes.begin(), selected_scenes.end(), scenes.at(name)) == selected_scenes.end()) {
		MAYA_LOG("Warning (Scene::BeginScene): the name \"" << name << "\" cannot be found in Scene::selected_scenes");
		return;
	}
#endif
	Scene* scene = scenes.at(name);
	selected_scenes.erase(std::remove(selected_scenes.begin(), selected_scenes.end(), scene), selected_scenes.end());
	scene->WhenEnd();
}

bool Scene::IsSceneSelected(std::string const& name)
{
#if MAYA_DEBUG
	if (!scenes.count(name)) {
		MAYA_LOG("Warning (Scene::BeginScene): the name \"" << name << "\" cannot be found in Scene::scenes");
		return false;
	}
#endif
	return std::find(selected_scenes.begin(), selected_scenes.end(), scenes.at(name)) != selected_scenes.end();
}

}