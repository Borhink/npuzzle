#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "shaders.h"

static GLint	get_program_log(GLuint shader_id)
{
	GLint	result;
	char	*buf;
	int		info_log_length;

	glGetProgramiv(shader_id, GL_LINK_STATUS, &result);
	if (result == 0)
	{
		glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		if (!info_log_length)
			return (result);
		buf = (char*)malloc((size_t)info_log_length);
		glGetProgramInfoLog(shader_id, info_log_length, NULL, buf);
		std::cout << buf << '\n';
		free(buf);
	}
	return (result);
}

static GLint	get_shader_log(GLuint shader_id)
{
	GLint	result;
	char	*buf;
	int		info_log_length;

	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (result == 0)
	{
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		if (!info_log_length)
			return (result);
		buf = (char*)malloc((size_t)info_log_length);
		glGetShaderInfoLog(shader_id, info_log_length, NULL, buf);
		std::cout << buf << '\n';
		free(buf);
	}
	return (result);
}


static std::string	getFileContent(const char *path)
{
	std::ifstream ifs(path);
  	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	return (content);
}

Shaders::Shaders() : _program(0), _vertex(0), _fragment(0), _geometry(0)
{}

Shaders::~Shaders()
{
	if (_program)
		glDeleteProgram(_program);
}

int				Shaders::loadVertexShader(const char *path)
{
	if (!path)
		throw std::runtime_error("(Vertex Shader) Le Chemin du fichier est NULL.\n");
	std::string script = getFileContent(path);
	if (script.length() <= 0)
		throw std::runtime_error("(Vertex Shader) Erreur lecture du fichier\n");
	_vertex = glCreateShader(GL_VERTEX_SHADER);
	const char	*str = script.c_str();
	glShaderSource(_vertex, 1, &str, NULL);
	glCompileShader(_vertex);
	return (get_shader_log(_vertex));
}

int				Shaders::loadFragmentShader(const char *path)
{
	if (!path)
		throw std::runtime_error("(Fragment Shader) Le Chemin du fichier est NULL.\n");
	std::string script = getFileContent(path);
	if (script.length() <= 0)
		throw std::runtime_error("(Vertex Shader) Erreur lecture du fichier\n");
	_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	const char	*str = script.c_str();
	glShaderSource(_fragment, 1, &str, NULL);
	glCompileShader(_fragment);
	return (get_shader_log(_fragment));
}

int				Shaders::loadGeometryShader(const char *path)
{
	if (!path)
		throw std::runtime_error("(Geometry Shader) Le Chemin du fichier est NULL.\n");
	std::string script = getFileContent(path);
	if (script.length() <= 0)
		throw std::runtime_error("(Geometry Shader) Erreur lecture du fichier\n");
	_geometry = glCreateShader(GL_GEOMETRY_SHADER);
	const char	*str = script.c_str();
	glShaderSource(_geometry, 1, &str, NULL);
	glCompileShader(_geometry);
	return (get_shader_log(_geometry));
}


int				Shaders::build(void)
{
	_program = glCreateProgram();
	if (!_program)
		return (0);
	glAttachShader(_program, _vertex);
	glAttachShader(_program, _fragment);
	if (_geometry)
		glAttachShader(_program, _geometry);
	glLinkProgram(_program);
	if (!get_program_log(_program))
		return (0);
	glDetachShader(_program, _vertex);
	glDetachShader(_program, _fragment);
	if (_geometry)
		glDetachShader(_program, _geometry);
	if (_vertex)
		glDeleteShader(_vertex);
	if (_fragment)
		glDeleteShader(_fragment);
	if (_geometry)
		glDeleteShader(_geometry);
	return (1);
}

void			Shaders::bind(void)
{
	glUseProgram(_program);
}

void			Shaders::uniformMat4(GLchar *name, GLfloat *mat)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniformMatrix4fv(id, 1, GL_FALSE, mat);
}

void			Shaders::uniform1f(GLchar *name, GLfloat value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform1f(id, value);
}

void			Shaders::uniform2f(GLchar *name, GLfloat *value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform2fv(id, 1, value);
}

void			Shaders::uniform2fv(GLchar *name, GLfloat x, GLfloat y)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform2f(id, x, y);
}

void			Shaders::uniform3f(GLchar *name, GLfloat *value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform3fv(id, 1, value);
}

void			Shaders::uniform4f(GLchar *name, GLfloat *value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform4fv(id, 1, value);
}

void			Shaders::uniform1i(GLchar *name, GLint value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform1i(id, value);
}

void			Shaders::uniform1ia(GLchar *name, GLsizei count, const GLint *value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform1iv(id, count, value);
}
