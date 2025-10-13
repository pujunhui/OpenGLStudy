#version 460 core

out vec4 FragColor;
in vec3 vColor;
in vec2 vUV;

uniform sampler2D sampler;

void main()
{
    FragColor = texture(sampler, vUV);
}