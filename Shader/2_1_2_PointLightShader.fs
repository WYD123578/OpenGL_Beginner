#version 330 core

struct PointLight {
	vec3 color;
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Material {
	sampler2D texture;
    sampler2D specular;

    float shininess;
};

in vec3 WorldFragPos;
in vec3 WorldNormal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 worldCameraPos;
uniform Material material;
uniform PointLight light;

void main()
{
	// ��ͼ����
	vec3 texSample = vec3(texture(material.texture, TexCoords));
	vec3 specSample = vec3(texture(material.specular, TexCoords));

	// ���㻷����˥��
	float distance = length(light.position - WorldFragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));
	
	// ====== ������ռ��¼��� ======
	// albedo
	vec3 albedo = texSample;

	// ambient
	vec3 ambient = (light.color * light.ambient) * albedo * attenuation; // ��ɫϵ��һ�������������ܹ������ջ���˵��������ĸ���ɫ����

	// diffuse
	vec3 norm = normalize(WorldNormal);
	vec3 lightDir = normalize(light.position - WorldFragPos); // ��Ϊ�������ʱ���Ǵ�ƬԪָ���Դ�ģ�������Ĺ�Դ�����ǹ�Դָ��ƬԪ��
	vec3 diffuse = (light.color * light.diffuse) * albedo * (0.5 + 0.5 * dot(norm, lightDir)) * attenuation;

	// specular
	vec3 viewDir = normalize(worldCameraPos-WorldFragPos);
	vec3 reflectVec = normalize(reflect(-lightDir, norm));
	vec3 specular = (light.color * light.specular) * specSample * pow(max(dot(reflectVec, viewDir), 0.0f), material.shininess) * attenuation;
	
	// ���屾�����ɫ���������������RGB���������ķ����ʣ����Խ�����ƹ���ɫ��ˣ��ű�ʾ�Ĺ�����������ϵ���ɫ
	FragColor = vec4((diffuse + ambient + specular), 1.0);
}