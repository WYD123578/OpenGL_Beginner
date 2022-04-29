#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec2 TexCoords;

uniform vec3 lightPos; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 trans;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

    // 顶点坐标到视图坐标
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;

    // 输入的光线的坐标就是世界坐标，所以不用乘模型矩阵
    LightPos = vec3(view * vec4(lightPos, 1.0));

    // UV顶点坐标
    TexCoords = aTexCoords;
}