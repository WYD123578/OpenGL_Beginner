#version 330 core
layout (points) in;
// 设置最大顶点数，超过后
layout (line_strip, max_vertices = 2) out;

void main()
{
    // gl_in在处理顶点points输入的时候只有一个图元数据
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
    EmitVertex();
    
    EndPrimitive();
}