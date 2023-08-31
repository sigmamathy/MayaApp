#pragma once

#include "./math.hpp"

namespace Maya {

class Scene
{
public:
	virtual void OnBegin() {}
	virtual void OnClose() {}
	virtual void OnTick(float elapsed) {}
	virtual void OnEvent(Event const& e) {}
};

template<class Ty> requires std::is_base_of_v<Scene, Ty>
void AssignScene(std::string const& name) {
	AssignScene_impl(name, std::make_unique<Ty>());
}

void AssignScene_impl(std::string const& name, std::unique_ptr<Scene>&& scene);

void SelectScene(std::string const& name);

}