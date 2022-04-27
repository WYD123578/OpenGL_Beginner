#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewPos;
// ƽ�й�Դ����
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

	// ���Դ�����˥��������һ��˥��ϵ����ϵ��Խ��˥��Խ����
	float energyDecay = dot(light, light) * decayRate;
	
	vec3 diffuse = lightColor * max(dot(norm, lightDir), 0.0);
	// һ����Χ��˥��
	if(energyDecay > 0.1f)
	{
		//diffuse = diffuse / energyDecay;
	}

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfVec = normalize(viewDir + lightDir);
	vec3 specular = 0.5 * lightColor * pow(max(dot(halfVec, norm), 0.0f), specularDecayRate);
	
	// ���屾�����ɫ���������������RGB���������ķ����ʣ����Խ�����ƹ���ɫ��ˣ��ű�ʾ�Ĺ�����������ϵ���ɫ
	FragColor = vec4((diffuse + ambient + specular) * objectColor, 1.0);
}