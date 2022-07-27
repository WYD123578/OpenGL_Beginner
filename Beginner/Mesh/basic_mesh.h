#include "Mesh.h"

namespace Beginner
{
	const std::vector<Vertex> SQUARE_VERTEX = {
		{glm::vec3( 0.5,  0.5, 0), glm::vec3(0, 0, 1), glm::vec2(1, 1)},
		{glm::vec3( 0.5, -0.5, 0), glm::vec3(0, 0, 1), glm::vec2(1, 0)},
		{glm::vec3(-0.5,  0.5, 0), glm::vec3(0, 0, 1), glm::vec2(0, 1)},
		{glm::vec3(-0.5, -0.5, 0), glm::vec3(0, 0, 1), glm::vec2(0, 0)},
	};

	const std::vector<unsigned int> SQUARE_INDICES = {0, 1, 2, 1, 2, 3,};

	const std::vector<Vertex> PLANE_VERTEX = {
		{glm::vec3( 5.0f, -0.5f,  5.0f), glm::vec3(0, 0, 1), glm::vec2(2.0f, 0.0f)},
		{glm::vec3(-5.0f, -0.5f,  5.0f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-5.0f, -0.5f, -5.0f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 2.0f)},
		{glm::vec3( 5.0f, -0.5f, -5.0f), glm::vec3(0, 0, 1), glm::vec2(2.0f, 2.0f)},
	};

	const std::vector<unsigned int> PLANE_INDICES = { 0, 1, 2, 0, 2, 3, };
}