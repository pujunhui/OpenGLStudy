#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

out vec3 color;
out vec2 uv;

uniform float time;

void main()
{
	float dx = 0.3;
	float speed = 5.0;
	float offsetX = sin(time * speed) * dx;
	//gl_Position = vec4(aPos.x + offsetX, aPos.y, aPos.z, 1.0);

	gl_Position = vec4(aPos, 1.0);
	color = aColor;

//	float deltaU = time * 0.3;
//	uv = vec2(aUV.x + deltaU, aUV.y);

	uv = aUV;
}