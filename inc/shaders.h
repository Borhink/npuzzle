#ifndef SHADERS_HPP
# define SHADERS_HPP

# include "struct.h"

class Shaders {
private:
	GLuint			_program;
	GLuint			_vertex;
	GLuint			_fragment;
	GLuint			_geometry;

public:
	Shaders();
	~Shaders();
	int				loadVertexShader(const char *path);
	int				loadFragmentShader(const char *path);
	int				loadGeometryShader(const char *path);
	int				build(void);
	void			bind(void);
	void			uniformMat4(GLchar *name, GLfloat *mat);
	void			uniform1f(GLchar *name, GLfloat value);
	void			uniform2f(GLchar *name, GLfloat *value);
	void			uniform2fv(GLchar *name, GLfloat x, GLfloat y);
	void			uniform3f(GLchar *name, GLfloat *value);
	void			uniform4f(GLchar *name, GLfloat *value);
	void			uniform1i(GLchar *name, GLint value);
	void			uniform1ia(GLchar *name, GLsizei count, const GLint *value);
};
#endif
