#version 460 core

in vec2 uv;
in vec3 normal;

uniform sampler2D sampler;

//光源参数
uniform vec3 lightDirection;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
	//1 获取物体的当前像素的颜色
	vec3 objectColor = texture(sampler, uv).xyz;

	//2 准备diffuse（漫反射）相关的各类数据
	vec3 normalN = normalize(normal);
	vec3 lightDirN = normalize(lightDirection);

	//3 计算漫反射cos值
	float diffuse = dot(-lightDirN, normalN);
	
	//4 避免光照背面cos值
	diffuse = clamp(diffuse, 0.0f, 1.0f);

	vec3 finalColor = lightColor * diffuse * objectColor;
	
	FragColor = vec4(finalColor, 1.0f);
}

