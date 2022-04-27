#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

// 平行光源方向
// uniform vec3 lightDir;

uniform float ambientStrength;
uniform float decayRate;

uniform float specularDecayRate;

void main()
{
	// ambient
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	vec3 diffuse = lightColor * max(dot(norm, lightDir), 0.0);

	// specular
	vec3 viewDir = normalize(-FragPos); // 视图空间中，视点位置一直是(0,0,0)
	vec3 halfVec = normalize(viewDir + lightDir);
	vec3 specular = 0.5 * lightColor * pow(max(dot(halfVec, norm), 0.0f), specularDecayRate);
	
	// 物体本身的颜色，描述的是物体对RGB各个分量的反射率，所以将其与灯光颜色相乘，才表示的光在这个材质上的颜色
	FragColor = vec4((diffuse + ambient + specular) * objectColor, 1.0);
}