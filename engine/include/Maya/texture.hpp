#pragma once

#include "./math.hpp"

namespace Maya {

class Texture final
{
public:
	MAYA_NO_COPY_CONSTRUCT(Texture);

	// Pass the image data into this texture
	Texture(std::uint8_t const* data, Ivec2 size, int channels);

	// Directly load the texture from a file
	Texture(std::string const& path, int channels = 0);

	~Texture();

	// Bind the texture to the slot specified
	void Bind(int slot);

private:
	std::uint32_t textureid;
	Ivec2 size;
};

}