#version 330 core
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexTex;

uniform vec2 windowSize;
uniform vec2 scale;
uniform vec2 pos;
uniform vec2 scaleTex;
uniform vec2 posTex;

out vec3 vColor;
out vec2 vTex;

void main()
{
	vec2 halfSize = windowSize / 2.0;
	vec2 inScreen = ((vertexPosition * scale + pos) - halfSize) / vec2(halfSize.x, -halfSize.y);
	gl_Position = vec4(inScreen, 0, 1);
	vColor = vertexColor;
	vTex = (vec2(vertexTex.x, vertexTex.y) * scaleTex) + posTex;
	vTex = vec2(vTex.x, 1.0 - vTex.y);
}
