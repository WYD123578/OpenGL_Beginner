#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

out vec4 FragColor;

// ƽ�й�Դ����
// uniform vec3 lightDir;

struct Light {
	
	vec3 color;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float decayRate;
};

struct Material {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
};

uniform Material material;
uniform Light light;

void main()
{
	// ambient
	vec3 ambient = light.color * light.ambient * material.ambient; // ��ɫϵ��һ�������������ܹ������ջ���˵��������ĸ���ɫ����

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	vec3 diffuse = light.color * light.diffuse * material.diffuse * max(dot(norm, lightDir), 0.0);

	// specular
	vec3 viewDir = normalize(-FragPos); // ��ͼ�ռ��У��ӵ�λ��һֱ��(0,0,0)
	vec3 reflectVec = normalize(reflect(-lightDir, norm));
	vec3 specular = light.color * light.specular * material.specular * pow(max(dot(reflectVec, viewDir), 0.0f), material.shininess);
	
	// ���屾�����ɫ���������������RGB���������ķ����ʣ����Խ�����ƹ���ɫ��ˣ��ű�ʾ�Ĺ�����������ϵ���ɫ
	FragColor = vec4((diffuse + ambient + specular), 1.0);
}