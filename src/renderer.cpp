#include "renderer.h"

Renderer::Renderer(float width, float height) :
	_width(width), _height(height)
{}


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
