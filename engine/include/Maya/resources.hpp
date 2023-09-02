#pragma once

#include "./vertex_array.hpp"
#include "./shader.hpp"
#include "./texture.hpp"

namespace Maya {

template<class ResourcesPtrTy>
void Assign(std::string const& name, ResourcesPtrTy ptr);

VertexArray& GetVertexArray(std::string const& name);

Shader& GetShader(std::string const& name);

Texture& GetTexture(std::string const& name);

}