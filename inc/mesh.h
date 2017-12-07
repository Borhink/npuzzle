#ifndef MESH_HPP
# define MESH_HPP

# include "struct.h"

class Mesh {
private:
	unsigned int	_vboNumber;
	GLuint			_vao;
	GLuint			*_vbo;
	unsigned int	_size;

public:
	Mesh(unsigned int vboNumber = 0);
	~Mesh();
	bool			isCreated(void);
	void			begin(void);
	void			add(GLuint index, GLenum type, size_t width, void *data, size_t dataSize, GLenum usage = GL_STATIC_DRAW);
	void			end(void);
	void			render(GLenum mode = GL_TRIANGLES);
};

#endif
