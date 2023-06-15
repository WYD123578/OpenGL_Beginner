#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 WorldFragPos;
out vec3 WorldNormal;
out vec2 TexCoords;

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

uniform mat4 model;

void main()
{
    vec3 offsetPos = aPos + aNormal*0.1f;
	gl_Position = projection * view * model * vec4(offsetPos, 1.0);

    // 顶点坐标到世界坐标
    WorldFragPos = vec3(model * vec4(aPos, 1.0));
    WorldNormal = mat3(transpose(inverse(model))) * aNormal;

    // UV顶点坐标
    TexCoords = aTexCoords;
}