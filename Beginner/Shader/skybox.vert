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
    TexCoords = aPos; // 物体的中心到顶点的方向，（数值上）就是顶点的坐标，不能归一化，因为纹理可以是(1,1)
    vec4 pos = projection * viewNew * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}