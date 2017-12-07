#version 330 core

uniform sampler2D uTexture;

out vec4 color;

in vec3 vColor;
in vec2 vTex;

void main()
{
	vec4 texColor = texture(uTexture, vTex);
	color = vec4(vColor, 1.0) * texColor;
}
