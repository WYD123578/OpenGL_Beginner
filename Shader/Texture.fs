#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

void main()
{
	if (TexCoord.x > 0.5 && TexCoord.y < 0.5)
	{
		FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x * 2, TexCoord.y * 2)), mixValue);
	}
	else
	{
		FragColor = texture(texture1, TexCoord);
	}
}