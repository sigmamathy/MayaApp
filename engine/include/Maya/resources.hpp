#pragma once

#include "./vertex_array.hpp"
#include "./shader.hpp"
#include "./texture.hpp"
#include "./font.hpp"
#include "./audio_stream.hpp"

namespace Maya {

template<class ResourcesPtrTy>
void Assign(std::string const& name, ResourcesPtrTy ptr);

VertexArray& GetVertexArray(std::string const& name);

Shader& GetShader(std::string const& name);

Texture& GetTexture(std::string const& name);

Font& GetFont(std::string const& name);

AudioStream& GetAudioStream(std::string const& name);

}