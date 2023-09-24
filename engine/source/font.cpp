#include <Maya/font.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Maya {

Font::Font(std::string const& path, int pixel_size)
{
    FT_Library ft;
    FT_Init_FreeType(&ft);
    FT_Face face;
    FT_New_Face(ft, path.c_str(), 0, &face);
    FT_Set_Pixel_Sizes(face, 0, pixel_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 32; c < 128; c++)
    {
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        auto& map = face->glyph->bitmap;

        std::vector<unsigned char> image;
        image.reserve(map.width * map.rows * 4);

        for (unsigned j = 0; j < map.rows; j++)
            for (unsigned i = 0; i < map.width; i++)
                for (unsigned k = 0; k < 4; k++)
                    image.emplace_back(map.buffer[(map.rows - j - 1) * map.width + i]);

        Texture* tex = new Texture(image.data(), Ivec2(map.width, map.rows), 4);
        glyphs[c] = {
            tex, Ivec2(map.width, map.rows),
            Ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            unsigned(face->glyph->advance.x)
        };
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Font::~Font()
{
    for (auto& pair : glyphs)
        delete pair.second.texture;
}

Glyph Font::operator[](unsigned char c) const
{
    return glyphs.at(c);
}

}