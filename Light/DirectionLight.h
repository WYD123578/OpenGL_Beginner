#pragma once
#include "Light.h"

class DirectionLight : public Light
{
public:

	glm::vec3 direction;
};
