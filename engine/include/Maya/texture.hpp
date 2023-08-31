#pragma once

#include "./math.hpp"

namespace Maya {

class Texture final
{
public:
	void Bind(int slot);

private:
	std::uint32_t textureid;
	Ivec2 size;

private:
	Texture(std::string const& path, int channels);
	Texture(Texture const&) = delete;
	Texture& operator=(Texture const&) = delete;
	~Texture();
	void Bind(int slot);
	friend class PrivateControl;
};

void AssignTexture(std::string const& name, std::string const& path, int channels = 0);

void UseTexture(std::string const& name, int slot = 0);

}