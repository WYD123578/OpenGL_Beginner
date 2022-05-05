#version 330 core

struct SpotLight {
	vec3 color;
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 direction;
	float cutOff;
	float outerCutOff;
};

struct Material {
	sampler2D texture;
    sampler2D specular;

    float shininess;
};

in vec3 WorldFragPos;
in vec3 WorldNormal;
in vec2 TexCoords;

uniform vec3 worldCameraPos;
uniform Material material;
uniform SpotLight light;

out vec4 FragColor;

void main()
{
	// ��ͼ����
	vec3 texSample = vec3(texture(material.texture, TexCoords));
	vec3 specSample = vec3(texture(material.specular, TexCoords));
	
	// ====== ������ռ��¼��� ======
	vec3 lightDir = normalize(light.position - WorldFragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    

	// albedo
	vec3 albedo = texSample;

	// ambient
	vec3 ambient = (light.color * light.ambient) * albedo; // ��ɫϵ��һ�������������ܹ������ջ���˵��������ĸ���ɫ����

	// diffuse
	vec3 norm = normalize(WorldNormal);
	vec3 diffuse = (light.color * light.diffuse) * albedo * (0.5 + 0.5 * dot(norm, lightDir));

	// specular
	vec3 viewDir = normalize(worldCameraPos-WorldFragPos);
	vec3 reflectVec = normalize(reflect(-lightDir, norm));
	vec3 specular = (light.color * light.specular) * specSample * pow(max(dot(reflectVec, viewDir), 0.0f), material.shininess);
	
	// ���屾�����ɫ���������������RGB���������ķ����ʣ����Խ�����ƹ���ɫ��ˣ��ű�ʾ�Ĺ�����������ϵ���ɫ
	FragColor = vec4((ambient + diffuse * intensity + specular * intensity), 1.0);
	
}