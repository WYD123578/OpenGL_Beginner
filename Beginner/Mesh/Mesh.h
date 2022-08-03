#pragma once

#include <utility>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Beginner/Tool/Shader.h"

namespace Beginner
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		Mesh(std::vector<float> point, int posCount, bool hasNormal, bool hasCoords)
		{
			vertices = std::vector<Vertex>();
			int basicPointCount = posCount;
			if (hasNormal) basicPointCount += posCount;
			if (hasCoords) basicPointCount += 2;

			if (point.size() % basicPointCount != 0)
			{
				std::cout << "错误的节点数量，不够组成顶点\n";
				return;
			}

			for (unsigned long long i = 0; i < point.size(); i += basicPointCount)
			{
				auto offsset = i;
				glm::vec3 normal = glm::vec3();
				glm::vec2 coords = glm::vec3();
				glm::vec3 pos = glm::vec3();

				if (posCount == 2)
				{
					pos = glm::vec3(point[offsset], point[offsset + 1], 0);
				}
				else
				{
					pos = glm::vec3(point[offsset], point[offsset + 1], point[offsset + 2]);
				}
				offsset += posCount;

				if (hasNormal)
				{
					if (posCount == 2)
					{
						normal = glm::vec3(point[offsset], point[offsset + 1], 0);
					}
					else
					{
						normal = glm::vec3(point[offsset], point[offsset + 1], point[offsset + 2]);
					}
					offsset += posCount;
				}

				if (hasCoords)
				{
					coords = glm::vec2(point[offsset], point[offsset + 1]);
					offsset += 2;
				}

				vertices.push_back(Vertex{pos, normal, coords});
			}

			setupMesh();
		}

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		{
			this->vertices = std::move(vertices);
			this->indices = std::move(indices);

			setupMesh();
		}

		void draw(const Shader& shader) const
		{
			shader.use();
			// 绘制网格
			glBindVertexArray(VAO);
			if (indices.empty())
			{
				glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			}
			else
			{
				glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
			}
			glBindVertexArray(0);
		}

	private:
		unsigned int VAO, VBO, EBO;

		void setupMesh()
		{
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			if (!indices.empty())
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0],
				             GL_STATIC_DRAW);
			}

			// 顶点位置
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));

			// 法线位置
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 3));

			// UV坐标位置
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(sizeof(float) * 6));

			// 取消绑定，避免误操作
			glBindVertexArray(0);
		}
	};
}
