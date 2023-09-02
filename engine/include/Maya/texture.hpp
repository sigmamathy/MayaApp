#pragma once

#include "./math.hpp"

namespace Maya {

class Texture final
{
public:
	// Pass the image data into this texture
	// @param data: the array pointer of image
	// @param size: size of that image
	// @param channels: number of channels of image, expects 1 to 4
	Texture(std::uint8_t const* data, Ivec2 size, int channels);

	// Directly load the texture from file
	// @param channels: number of channels to be loaded (1 to 4), uses default if zero
	Texture(std::string const& path, int channels = 0);

	// Specify which texture to use
	// @param texture: could be nullptr if no texture are used
	// @param slot: indicates which texture slot to bind
	void Bind(int slot);

private:
	std::uint32_t textureid;
	Ivec2 size;

private:
	Texture(Texture const&) = delete;
	Texture& operator=(Texture const&) = delete;
	~Texture();
	friend class PrivateControl;
	friend class Font;
};

}