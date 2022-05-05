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
	// 贴图采样
	vec3 texSample = vec3(texture(material.texture, TexCoords));
	vec3 specSample = vec3(texture(material.specular, TexCoords));

	// 计算环境光衰减
	float distance = length(light.position - WorldFragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));
	
	// ====== 在世界空间下计算 ======
	// albedo
	vec3 albedo = texSample;

	// ambient
	vec3 ambient = (light.color * light.ambient) * albedo * attenuation; // 减色系，一束光在物体上能够被吸收或者说反射出来的各颜色分量

	// diffuse
	vec3 norm = normalize(WorldNormal);
	vec3 lightDir = normalize(light.position - WorldFragPos); // 因为我们算的时候都是从片元指向光源的，而定义的光源方向是光源指向片元的
	vec3 diffuse = (light.color * light.diffuse) * albedo * (0.5 + 0.5 * dot(norm, lightDir)) * attenuation;

	// specular
	vec3 viewDir = normalize(worldCameraPos-WorldFragPos);
	vec3 reflectVec = normalize(reflect(-lightDir, norm));
	vec3 specular = (light.color * light.specular) * specSample * pow(max(dot(reflectVec, viewDir), 0.0f), material.shininess) * attenuation;
	
	// 物体本身的颜色，描述的是物体对RGB各个分量的反射率，所以将其与灯光颜色相乘，才表示的光在这个材质上的颜色
	FragColor = vec4((diffuse + ambient + specular), 1.0);
}