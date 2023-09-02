#pragma once

#include "./texture.hpp"

namespace Maya {

// Stores the glyph data of each character
struct Glyph final
{
	Texture* texture;
	Ivec2 size, bearing;
	unsigned advance;
};

class Font final
{
public:
	// Load a truetype font through a filepath
	// @param pixel_size: the size of the pixel to be loaded
	Font(std::string const& path, int pixel_size);

	// Get a character glyph from the font
	Glyph operator[](unsigned char c) const;

private:
	std::unordered_map<unsigned char, Glyph> glyphs;
	~Font();
	Font(Font const&) = delete;
	Font& operator=(Font const&) = delete;
	friend class PrivateControl;
};

}