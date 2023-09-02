#include "./private_control.hpp"

namespace Maya {

void CreateScene_impl(std::string const& name, std::unique_ptr<Scene>&& scene)
{
	auto& ctrl = PrivateControl::Instance();
	ctrl.scenes.emplace(name, std::move(scene));
}

void SelectScene(std::string const& name)
{
	auto& ctrl = PrivateControl::Instance();
	if (ctrl.current_scene) ctrl.current_scene->OnClose();
	ctrl.current_scene = ctrl.scenes.at(name).get();
	ctrl.current_scene->OnBegin();
	auto* crnt = ctrl.current_scene;
	ctrl.windata.callback = [=](Event const& e) { crnt->OnEvent(e); };
}

}