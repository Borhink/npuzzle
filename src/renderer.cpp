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

	///////////// TRUC DEGUEULASSE A MOI ///////////
	char paths[6][256] = {"textures/univers.bmp", "textures/marvin.bmp", "textures/baleine.bmp", "textures/pikachu.bmp", "textures/dessin.bmp", "textures/mosaique.bmp"};

	Texture	*texture = Texture::LoadBMP(paths[std::rand() % 6]);
	///////////// FIN TRUC DEGUEULASSE A MOI ///////////

	Board	*board = npuzzle->getBoard();
	int		mapSize = board->size();
	float	renderScale = 720.0f / (float)mapSize;
	float	offsetx = 640.0f - ((renderScale * (float)mapSize) / 2.0f);

	float	textureScale = 1.0f / (float)mapSize;

	std::vector<glm::ivec2>			solvedMap = npuzzle->getSolvedMap();
	std::vector<std::vector<int>>	map = board->getMap();
	std::vector<std::vector<int>>	lastMap = map;
	Astar 							*as = npuzzle->getAStar();
	std::stack<class Node*> path;
	bool					finish = false;
	// int 					sleepTime = 100000;
	const double			maxTimeAnim = npuzzle->getAnimInMS() / 1000.0f;
	double					sleepAnim = maxTimeAnim;
	double					lastTime = glfwGetTime();
	glm::ivec2				itemPos;
	glm::ivec2				itemDir;
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
		{
			as = npuzzle->getAStar();
			lastTime = glfwGetTime();
		}
		
		double now = glfwGetTime();
		double elapse = now - lastTime;

		if (as != nullptr && as->isSolved())
		{
			if (path.empty() && !finish)
			{
				as->restorePath(path);
				///////////// TRUC DEGUEULASSE A MOI ///////////
				if (!path.empty())
				{
					// sleepTime = 10000000 / path.size();
					// if (sleepTime > 1000000)
					// 	sleepTime = 1000000;
					sleepAnim = maxTimeAnim; // (double)path.size();
					if (sleepAnim > maxTimeAnim)
						sleepAnim = maxTimeAnim;
					elapse = now - lastTime;
				}
				///////////// FIN TRUC DEGUEULASSE A MOI ///////////
			}
			if (!path.empty() && elapse >= sleepAnim)
			{
				class Node *node = path.top();
				class Node *parent = node->getParent();
				class Board *board = node->getBoard();
				lastMap = map;
				map = board->getMap();
				mapCompare(lastMap, map, mapSize, &itemPos, &itemDir);
				path.pop();
				if (path.empty())
					finish = true;
				lastTime += sleepAnim;
				elapse = now - lastTime;
			}
		}
		double pctMove = elapse / sleepAnim;
		if (pctMove < 0.0)
			pctMove = 0.0;
		if (pctMove > 1.0)
			pctMove = 1.0;
		for (int y = 0; y < mapSize; y++)
		{
			for (int x = 0; x < mapSize; x++)
			{
				///////////// TRUC DEGUEULASSE A MOI ///////////
				if (finish && map[y][x] == 0 && pctMove >= 1.0)
				{
					glm::ivec2 current = solvedMap[map[y][x]];
					mainShader.uniform2fv((GLchar *)"pos", offsetx + x * renderScale, y * renderScale);
					mainShader.uniform2fv((GLchar *)"posTex", current[0] * textureScale, current[1] * textureScale);
					mesh.render();
				}
				///////////// FIN TRUC DEGUEULASSE A MOI ///////////
				if (map[y][x] != 0)
				{
					double	offx = 0.0f;
					double	offy = 0.0f;
					if (itemPos[0] == x && itemPos[1] == y)
					{
						offx = itemDir[0] * (1.0 - pctMove);
						offy = itemDir[1] * (1.0 - pctMove);
					}
					glm::ivec2 current = solvedMap[map[y][x]];
					mainShader.uniform2fv((GLchar *)"pos", offsetx + ((double)x + offx) * renderScale, ((double)y + offy) * renderScale);
					mainShader.uniform2fv((GLchar *)"posTex", current[0] * textureScale, current[1] * textureScale);
					mesh.render();
				}
			}
		}
	    glfwSwapBuffers(_window);
	    glfwPollEvents();
	}
	delete texture;
}

void				Renderer::mapCompare(std::vector<std::vector<int>> map0, std::vector<std::vector<int>> map1, int size, glm::ivec2 *pos, glm::ivec2 *dir)
{
	glm::ivec2	p2(-1, -1);
	(*pos)[0] = -1;
	(*pos)[1] = -1;
	int ok = 0;
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			if (map0[y][x] == 0)
			{
				(*pos)[0] = x;
				(*pos)[1] = y;
				ok++;
			}
			if (map1[y][x] == 0)
			{
				p2[0] = x;
				p2[1] = y;
				ok++;
			}
			if (ok >= 2)
				break;
		}
		if (ok >= 2)
			break;
	}
	(*dir)[0] = p2[0] - (*pos)[0];
	(*dir)[1] = p2[1] - (*pos)[1];
}