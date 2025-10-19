#version 460 core

in vec2 uv;
uniform sampler2D sampler;

out vec4 FragColor;

void main()
{
	FragColor = texture(sampler, uv);
}