#version 420 core
out vec4 FragColor;

in VS_OUT
{
    vec2 TexCoords;
} fs_in;

uniform sampler2D mainTexture;

void main()
{             
    FragColor = texture(mainTexture, fs_in.TexCoords);
}