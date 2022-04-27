#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewPos;
// 平行光源方向
// uniform vec3 lightDir;

uniform float ambientStrength;
uniform float decayRate;

uniform float specularDecayRate;

void main()
{
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);

	vec3 light = lightPos - FragPos;
	vec3 lightDir = normalize(light);

	// 点光源随距离衰减，定义一个衰减系数，系数越大，衰减越厉害
	float energyDecay = dot(light, light) * decayRate;
	
	vec3 diffuse = lightColor * max(dot(norm, lightDir), 0.0);
	// 一定范围内衰减
	if(energyDecay > 0.1f)
	{
		//diffuse = diffuse / energyDecay;
	}

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfVec = normalize(viewDir + lightDir);
	vec3 specular = 0.5 * lightColor * pow(max(dot(halfVec, norm), 0.0f), specularDecayRate);
	
	// 物体本身的颜色，描述的是物体对RGB各个分量的反射率，所以将其与灯光颜色相乘，才表示的光在这个材质上的颜色
	FragColor = vec4((diffuse + ambient + specular) * objectColor, 1.0);
}