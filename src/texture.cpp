#include "texture.h"

Texture::Texture(unsigned int width, unsigned int height, unsigned char *data, GLenum format) :
_width(width), _height(height), _data(data)
{
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, format, GL_UNSIGNED_BYTE, _data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	// delete[] data;
}

Texture::~Texture()
{
	delete[] _data;
	glDeleteTextures(1, &_textureID);
}

void		Texture::bind(void)
{
	glBindTexture(GL_TEXTURE_2D, _textureID);
}

Texture		*Texture::LoadBMP(const char *path, GLenum format)
{
	unsigned char	header[54];
	unsigned int	dataPos;
	unsigned int	width;
	unsigned int	height;
	unsigned int	imageSize;
	unsigned char	*data;

	FILE *file = fopen(path, "rb");
	if (!file)
		throw std::logic_error("Image could not be opened\n");
	if (fread(header, 1, 54, file) != 54)
		throw std::logic_error("Not a correct BMP file\n");
	if (header[0] != 'B' || header[1] != 'M')
		throw std::logic_error("Not a correct BMP file\n");
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (imageSize == 0)
		imageSize = width * height * 3;
	if (dataPos == 0)
		dataPos = 54;
	fseek(file, dataPos, SEEK_SET);
	data = new unsigned char [imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);
	return (new Texture(width, height, data, format));
}
