#pragma once

#include "./vertex_array.hpp"
#include "./shader.hpp"
#include "./texture.hpp"
#include "./font.hpp"
#include "./audio_stream.hpp"

namespace Maya {

class ResourcesManager
{
public:
	static ResourcesManager& Instance();

#define MAYA_RESOURCES_MANAGER_CREATE_FUNC(_ty, _list)\
	template<class... Tys> void Create##_ty(std::string const& name, Tys const&... args) {\
	_list[name] = _ty(args...); }

	MAYA_RESOURCES_MANAGER_CREATE_FUNC(VertexArray, vaos)
	MAYA_RESOURCES_MANAGER_CREATE_FUNC(Shader, shaders)
	MAYA_RESOURCES_MANAGER_CREATE_FUNC(Texture, textures)
	MAYA_RESOURCES_MANAGER_CREATE_FUNC(Font, fonts)
	MAYA_RESOURCES_MANAGER_CREATE_FUNC(AudioStream, audio_streams)

#undef MAYA_RESOURCES_MANAGER_CREATE_FUNC

#define MAYA_RESOURCES_MANAGER_GET_FUNC(_ty, _list)\
	_ty& Get##_ty(std::string const& name) { return _list.at(name); }

	MAYA_RESOURCES_MANAGER_GET_FUNC(VertexArray, vaos)
	MAYA_RESOURCES_MANAGER_GET_FUNC(Shader, shaders)
	MAYA_RESOURCES_MANAGER_GET_FUNC(Texture, textures)
	MAYA_RESOURCES_MANAGER_GET_FUNC(Font, fonts)
	MAYA_RESOURCES_MANAGER_GET_FUNC(AudioStream, audio_streams)

#undef MAYA_RESOURCES_MANAGER_GET_FUNC

private:
	std::unordered_map<std::string, VertexArray> vaos;
	std::unordered_map<std::string, Shader> shaders;
	std::unordered_map<std::string, Texture> textures;
	std::unordered_map<std::string, Font> fonts;
	std::unordered_map<std::string, AudioStream> audio_streams;

private:
	ResourcesManager();
	ResourcesManager(ResourcesManager const&) = delete;
	ResourcesManager& operator=(ResourcesManager const&) = delete;
};

}