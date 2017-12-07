#ifndef TEXTURE_HPP
#define TEXTURE_HPP

# include <string>
# include "struct.h"

# define FONT_LINE	8
# define FONT_ROW	16

class Texture {
private:
	unsigned int	_width;
	unsigned int	_height;
	unsigned char	*_data;
	GLuint			_textureID;
public:
	Texture(unsigned int width, unsigned int height, unsigned char *data, GLenum format = GL_BGR);
	~Texture();
	void	bind(void);
	inline GLuint getID(void) { return (_textureID); }

	static Texture	*LoadBMP(const char *path, GLenum format = GL_BGR);
	inline int	getWidth(void) { return (_width); }
	inline int	getHeight(void) { return (_height); }
};

#endif
