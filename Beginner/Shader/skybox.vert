#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

uniform mat4 viewNew;

void main()
{
    TexCoords = aPos; // ��������ĵ�����ķ��򣬣���ֵ�ϣ����Ƕ�������꣬���ܹ�һ������Ϊ���������(1,1)
    vec4 pos = projection * viewNew * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}