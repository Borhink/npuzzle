#include <stdexcept>
#include <unistd.h>
#include "board.h"
#include "renderer.h"
#include "mesh.h"
#include "shaders.h"
#include "texture.h"
#include "astar.h"

static void win_resize_callback(GLFWwindow *window, int width, int height)
{
	Renderer	*renderer;

	renderer = (Renderer *)glfwGetWindowUserPointer(window);
	if (!renderer)
		return ;
	renderer->setSize((float)width, (float)height).viewport();
}

Renderer::Renderer(float width, float height) :
	_width(width), _height(height)
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW\n");
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	_window = glfwCreateWindow((int)width, (int)height, "npuzzle", NULL, NULL);
	if (_window == NULL)
		throw std::runtime_error("Failed to open GLFW window. If you have an Intel GPU, they are not 4.1 compatible.\n");
	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, win_resize_callback);
	glfwSwapInterval(0);
	glewExperimental=true;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Failed to initialize GLEW\n");
	viewport();
}

Renderer::~Renderer()
{
	glfwTerminate();
}

Renderer			&Renderer::setWidth(float width)
{
	_width = width;
	return (*this);
}

Renderer			&Renderer::setHeight(float height)
{
	_height = height;
	return (*this);
}

Renderer			&Renderer::setSize(float width, float height)
{
	return (setWidth(width).setHeight(height));
}

Renderer			&Renderer::viewport(void)
{
	glViewport(0, 0, _width, _height);
	_projection = glm::perspective(glm::radians(45.0f), _width / _height, 0.001f, 100.0f);
	return (*this);
}

void				Renderer::loop(Npuzzle *npuzzle)
{
	Shaders	mainShader;
	if (!mainShader.loadFragmentShader("shaders/basic.frag")
		|| !mainShader.loadVertexShader("shaders/basic.vert")
		|| !mainShader.build())
		throw std::runtime_error("Error \"mainShader\" can't compile!\n");

	Mesh	mesh(3);
	mesh.begin();
	static const GLfloat vPos[] = {
		0.0f, 0.0f,		0.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 1.0f,		1.0f, 0.0f,		0.0f, 0.0f,
	};
	static const GLfloat vColor[] = {
		1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
	};
	static const GLfloat vTex[] = {
		0.0f, 0.0f,		0.0f, 1.0f,		1.0f, 1.0f,
		1.0f, 1.0f,		1.0f, 0.0f,		0.0f, 0.0f,
	};
	mesh.add(0, GL_FLOAT, 2, (void *)vPos, 6);
	mesh.add(1, GL_FLOAT, 3, (void *)vColor, 6);
	mesh.add(2, GL_FLOAT, 2, (void *)vTex, 6);
	mesh.end();

	Texture	*texture = Texture::LoadBMP("textures/marvin.bmp");

	Board	*board = npuzzle->getBoard();
	int		mapSize = board->size();
	float	renderScale = 720.0f / (float)mapSize;
	float	offsetx = 640.0f - ((renderScale * (float)mapSize) / 2.0f);

	float	textureScale = 1.0f / (float)mapSize;

	std::vector<glm::ivec2>			solvedMap = npuzzle->getSolvedMap();
	std::vector<std::vector<int>>	map = board->getMap();
	Astar 							*as = npuzzle->getAStar();
	std::stack<class Node*> path;
	bool					finish = false;
	int 					sleepTime = 100000;
	while (glfwGetKey(_window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& !glfwWindowShouldClose(_window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainShader.bind();
		mainShader.uniform2fv((GLchar *)"windowSize", 1280.0f, 720.0f);
		mainShader.uniform2fv((GLchar *)"scale", renderScale, renderScale);
		mainShader.uniform2fv((GLchar *)"scaleTex", textureScale, textureScale);
		mainShader.uniform1i((GLchar *)"uTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		texture->bind();
		if (as == nullptr)
			as = npuzzle->getAStar();
		if (as != nullptr && as->isSolved())
		{
			if (path.empty() && !finish)
			{
				as->restorePath(path);
				if (!path.empty())
				{
					sleepTime = 10000000 / path.size();
					if (sleepTime > 1000000)
						sleepTime = 1000000;
				}
			}
			if (!path.empty())
			{
				class Node *node = path.top();
				class Node *parent = node->getParent();
				class Board *board = node->getBoard();
				map = board->getMap();
				path.pop();
				if (path.empty())
					finish = true;
			}
		}
		for (int y = 0; y < mapSize; y++)
		{
			for (int x = 0; x < mapSize; x++)
			{
				if (map[y][x] != 0)
				{
					glm::ivec2 current = solvedMap[map[y][x]];
					mainShader.uniform2fv((GLchar *)"pos", offsetx + x * renderScale, y * renderScale);
					mainShader.uniform2fv((GLchar *)"posTex", current[0] * textureScale, current[1] * textureScale);
					mesh.render();
				}
			}
		}
	    glfwSwapBuffers(_window);
	    glfwPollEvents();
		usleep(sleepTime);
	}
	delete texture;
}
