#include <stdlib.h>
#include "mesh.h"

Mesh::Mesh(unsigned int vboNumber) : _vboNumber(vboNumber), _size(0)
{
	if (vboNumber)
	{
		if ((_vbo = (GLuint *)malloc(sizeof(GLuint) * vboNumber)))
		{
			glGenVertexArrays(1, &(_vao));
			glBindVertexArray(_vao);
			glGenBuffers(vboNumber, _vbo);
		}
	}
}

Mesh::~Mesh()
{
	if (_vbo)
	{
		glDeleteBuffers(_vboNumber, _vbo);
		free(_vbo);
	}
	if (_vao)
		glDeleteVertexArrays(1, &_vao);
}

bool			Mesh::isCreated(void)
{
	if (!_vao)
		return (false);
	if (!_vbo)
		return (false);
	for (size_t i = 0; i < _vboNumber; i++)
		if (!_vbo[i])
			return (false);
	return (true);
}

void			Mesh::begin(void)
{
	glBindVertexArray(_vao);
}

void			Mesh::add(GLuint index, GLenum type, size_t width, void *data, size_t dataSize, GLenum usage)
{
	size_t	s;
	if (type == GL_FLOAT)
		s = sizeof(GLfloat);
	else if (type == GL_INT)
		s = sizeof(GLint);
	else if (type == GL_BYTE)
		s = sizeof(GLbyte);
	else if (type == GL_UNSIGNED_BYTE)
		s = sizeof(GLubyte);
	else if (type == GL_SHORT)
		s = sizeof(GLshort);
	else if (type == GL_UNSIGNED_SHORT)
		s = sizeof(GLushort);
	else if (type == GL_UNSIGNED_INT)
		s = sizeof(GLuint);
	else if (type == GL_DOUBLE)
		s = sizeof(GLdouble);
	else
		s = sizeof(GLfloat);
	_size = dataSize;
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[index]);
	glBufferData(GL_ARRAY_BUFFER, s * width * dataSize, data, usage);
	if (type == GL_INT || type == GL_UNSIGNED_INT || type == GL_BYTE
		|| type == GL_UNSIGNED_BYTE || type == GL_SHORT || type == GL_UNSIGNED_SHORT)
		glVertexAttribIPointer(index, width, type, 0, (void *)0);
	else
		glVertexAttribPointer(index, width, type, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(index);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void			Mesh::end(void)
{
	glBindVertexArray(0);
}

void			Mesh::render(GLenum mode)
{
	glBindVertexArray(_vao);
	glDrawArrays(mode, 0, _size);
	glBindVertexArray(0);
}
