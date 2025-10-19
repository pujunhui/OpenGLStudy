#version 460 core

in vec2 uv;
in vec3 normal;

uniform sampler2D sampler;

out vec4 FragColor;

void main()
{
	//1 将vs输入的normal作一下归一化
	vec3 normalN = normalize(normal);
	
	//2 将负数的情况直接清理为0
	vec3 normalColor = clamp(normalN, 0.0f, 1.0f);
	
	FragColor = vec4(normalColor, 1.0f);
	//FragColor = texture(sampler, uv);
}

