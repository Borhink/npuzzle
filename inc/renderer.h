#ifndef RENDERER_H
# define RENDERER_H

# include <struct.h>

class Renderer {
private:
	GLFWwindow	*_window;
	glm::mat4	_projection;
	float		_width;
	float		_height;

public:
	Renderer(float width, float height);

	inline GLFWwindow	*getWindow(void) { return (_window); }
	inline glm::mat4	getProjection(void) { return (_projection); }
	inline float		getWidth(void) { return (_width); }
	inline float		getHeight(void) { return (_height); }
	Renderer			&setWidth(float width);
	Renderer			&setHeight(float height);
	Renderer			&setSize(float width, float height);
};

#endif
