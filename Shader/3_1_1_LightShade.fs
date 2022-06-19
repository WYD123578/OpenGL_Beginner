#version 330 core

struct DirectionLight {
	vec3 color;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 color;
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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
	sampler2D texture_diffuse_1;
	sampler2D texture_diffuse_2;
	sampler2D texture_diffuse_3;
	sampler2D texture_diffuse_4;

    sampler2D texture_specular_1;
    sampler2D texture_specular_2;
    sampler2D texture_specular_3;
    sampler2D texture_specular_4;

    float shininess;
};

uniform DirectionLight dirLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

in vec3 WorldFragPos;
in vec3 WorldNormal;
in vec2 TexCoords;

uniform vec3 worldCameraPos;
uniform Material material;

out vec4 FragColor;

vec4 CalcDirectionLight(Material mat, DirectionLight light, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(Material mat, PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir);
vec4 CalcSpotLight(Material mat, SpotLight light, vec3 fragPos, vec3 normal, vec3 viewDir);

void main()
{
	vec3 normal = normalize(WorldNormal);
	vec3 viewDir = normalize(worldCameraPos - WorldFragPos);

	vec4 result = CalcDirectionLight(material, dirLight, normal, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
       result += CalcPointLight(material, pointLights[i], normal, WorldFragPos, viewDir);

	FragColor = result;
}

vec4 CalcDirectionLight(Material mat, DirectionLight light, vec3 normal, vec3 viewDir) 
{
	vec3 lightDir = - normalize(light.direction);
	vec3 halfVec = normalize(lightDir + viewDir);
	
	// 材质采样实现的颜色
	vec3 albedo = vec3(texture(mat.texture_diffuse_1, TexCoords));
	vec3 ambient = light.ambient * light.color * albedo;
	// 半兰伯特模型实现的 Diffuse
	vec3 diffuse = light.diffuse * light.color * (0.5 * dot(lightDir, normal) + 0.5);
	// 半程向量实现的高光
	vec3 specular = light.specular * light.color * pow(max(dot(normal, halfVec), 0.0), material.shininess);

	vec4 col = vec4(ambient + diffuse + specular, 1.0f);
	return col;
}

vec4 CalcPointLight(Material mat, PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 halfVec = normalize(lightDir + viewDir);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));
	
	// 材质采样实现的颜色
	vec3 albedo = vec3(texture(mat.texture_diffuse_1, TexCoords));
	vec3 ambient = light.ambient * light.color * albedo;
	// 半兰伯特模型实现的 Diffuse
	vec3 diffuse = light.diffuse * light.color * (0.5 * dot(lightDir, normal) + 0.5);
	// 半程向量实现的高光
	vec3 specular = light.specular * light.color * pow(max(dot(normal, halfVec), 0.0), material.shininess);

	vec4 col = vec4(0f);
	return col;
}

vec4 CalcSpotLight(Material mat, SpotLight light, vec3 fragPos, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - WorldFragPos);
	vec3 halfVec = normalize(lightDir + viewDir);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);   
	
	// 材质采样实现的颜色
	vec3 albedo = vec3(texture(mat.texture_diffuse_1, TexCoords));
	vec3 ambient = light.ambient * light.color * albedo;
	// 半兰伯特模型实现的 Diffuse
	vec3 diffuse = light.diffuse * light.color * (0.5 * dot(lightDir, normal) + 0.5);
	// 半程向量实现的高光
	vec3 specular = light.specular * light.color * pow(max(dot(normal, halfVec), 0.0), material.shininess);

	vec4 col = vec4(ambient + (diffuse + specular) * intensity, 1.0f);
	return col;
}