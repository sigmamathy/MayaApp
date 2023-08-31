#include "./private_control.hpp"
#include <stb/stb_image.h>

namespace Maya {

static constexpr int texture_format(int channels)
{
	switch (channels)
	{
		case 1: return GL_RED;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default: return -1;
	}
}

Texture::Texture(std::string const& path, int channels)
{
	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_2D, textureid);

	stbi_set_flip_vertically_on_load(true);
	int ch;
	stbi_uc* data = stbi_load(path.c_str(), &size[0], &size[1], &ch, channels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size[0], size[1], 0,
		texture_format(channels == 0 || ch < channels ? ch : channels), GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);
	if (data) stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureid);
}

void Texture::Bind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureid);
}

void PrivateControl::AssignTexture_Temp(std::string const& name, std::string const& path, int channels)
{
	textures.emplace(name, new Texture(path, channels));
}

void AssignTexture(std::string const& name, std::string const& path, int channels)
{
	PrivateControl::Instance().AssignTexture_Temp(name, path, channels);
}

void UseTexture(std::string const& name, int slot)
{
	PrivateControl::Instance().textures.at(name)->Bind(slot);
}

}