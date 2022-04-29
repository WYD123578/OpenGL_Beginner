#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoords;

out vec4 FragColor;

// 平行光源方向
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
	// 贴图采样
	vec3 texSample = vec3(texture(material.texture, TexCoords));
	vec3 specSample = vec3(texture(material.specular, TexCoords));
	vec3 emissionSample = vec3(texture(material.emission, TexCoords));

	// 二值化高光，将不亮的部分剔除出来
	if(specSample.x > 0.0f)
	{
		emissionSample *= 1;
	}else{
		emissionSample *= 0;
	}

	// ambient
	vec3 ambient = light.color * light.ambient * texSample; // 减色系，一束光在物体上能够被吸收或者说反射出来的各颜色分量

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	vec3 diffuse = light.color * light.diffuse * texSample * max(dot(norm, lightDir), 0.0);

	// specular
	vec3 viewDir = normalize(-FragPos); // 视图空间中，视点位置一直是(0,0,0)
	vec3 reflectVec = normalize(reflect(-lightDir, norm));
	vec3 specular = light.color * light.specular * specSample * pow(max(dot(reflectVec, viewDir), 0.0f), material.shininess);
	
	// 物体本身的颜色，描述的是物体对RGB各个分量的反射率，所以将其与灯光颜色相乘，才表示的光在这个材质上的颜色
	FragColor = vec4((diffuse + ambient + specular + emissionSample), 1.0);
}