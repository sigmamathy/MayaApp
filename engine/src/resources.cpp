#include "./private_control.hpp"

namespace Maya {

ResourcesManager& ResourcesManager::Instance()
{
	static ResourcesManager manager;
	return manager;
}

ResourcesManager::ResourcesManager()
{
#if MAYA_DEBUG
	if (!PrivateControl::Instance().window) {
		std::cout << "CreateWindowInstance have not been called before ResourcesManager::Instance\n";
		throw 0;
	}
#endif
}

}