#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 worldCameraPos;
uniform samplerCube skybox;

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - worldCameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    if(gl_FrontFacing)
        FragColor = vec4(texture(skybox, R).rgb, 1.0);
    else
        FragColor = vec4(1,1,1,1);
}