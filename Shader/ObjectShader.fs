#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoords;

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
	sampler2D texture;
    sampler2D specular;
    sampler2D emission;

    float shininess;
};

uniform Material material;
uniform Light light;

void main()
{
	// ��ͼ����
	vec3 texSample = vec3(texture(material.texture, TexCoords));
	vec3 specSample = vec3(texture(material.specular, TexCoords));
	vec3 emissionSample = vec3(texture(material.emission, TexCoords));

	// ��ֵ���߹⣬�������Ĳ����޳�����
	if(specSample.x > 0.0f)
	{
		emissionSample *= 1;
	}else{
		emissionSample *= 0;
	}

	// ambient
	vec3 ambient = light.color * light.ambient * texSample; // ��ɫϵ��һ�������������ܹ������ջ���˵��������ĸ���ɫ����

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	vec3 diffuse = light.color * light.diffuse * texSample * max(dot(norm, lightDir), 0.0);

	// specular
	vec3 viewDir = normalize(-FragPos); // ��ͼ�ռ��У��ӵ�λ��һֱ��(0,0,0)
	vec3 reflectVec = normalize(reflect(-lightDir, norm));
	vec3 specular = light.color * light.specular * specSample * pow(max(dot(reflectVec, viewDir), 0.0f), material.shininess);
	
	// ���屾�����ɫ���������������RGB���������ķ����ʣ����Խ�����ƹ���ɫ��ˣ��ű�ʾ�Ĺ�����������ϵ���ɫ
	FragColor = vec4((diffuse + ambient + specular + emissionSample), 1.0);
}