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
	MAYA_NO_COPY_CONSTRUCT(Font);
	// Load a truetype font through a filepath
	Font(std::string const& path, int pixel_size);
	~Font();

	// Get a character glyph from the font
	Glyph operator[](unsigned char c) const;

private:
	std::unordered_map<unsigned char, Glyph> glyphs;
};

}