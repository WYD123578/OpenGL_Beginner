#pragma once
#include <glm/glm.hpp>

class Light
{
public:
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 color = glm::vec3(1, 1, 1);

	glm::vec3 ambient = glm::vec3(1,1,1);
	glm::vec3 diffuse = glm::vec3(1, 1, 1);
	glm::vec3 specular = glm::vec3(1, 1, 1);

};
