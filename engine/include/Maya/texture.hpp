#pragma once

#include "./math.hpp"

namespace Maya {

class Texture final
{
public:
	Texture(std::string const& path, int channels = 0);
	void Bind(int slot);

private:
	std::uint32_t textureid;
	Ivec2 size;

private:
	Texture(Texture const&) = delete;
	Texture& operator=(Texture const&) = delete;
	~Texture();
	friend class PrivateControl;
};

}