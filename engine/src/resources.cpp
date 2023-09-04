#include "./private_control.hpp"

namespace Maya {

#define MAYA_RESOURCES_FUNCTION(_ty, _list, _getfn)\
	template<> void Assign<_ty*>(std::string const& name, _ty* ptr)\
	{ PrivateControl::Instance()._list[name] = ptr; }\
	_ty& _getfn(std::string const& name) { return *PrivateControl::Instance()._list.at(name); }

MAYA_RESOURCES_FUNCTION(VertexArray, vaos, GetVertexArray)
MAYA_RESOURCES_FUNCTION(Shader, shaders, GetShader)
MAYA_RESOURCES_FUNCTION(Texture, textures, GetTexture)
MAYA_RESOURCES_FUNCTION(Font, fonts, GetFont)
MAYA_RESOURCES_FUNCTION(AudioStream, audio_streams, GetAudioStream)

}