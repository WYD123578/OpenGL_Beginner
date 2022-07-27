#pragma once

#include <string>
#include <vector>
#include <assimp/types.h>
#include "Beginner/Tool/Shader.h"

namespace Beginner
{
	struct Texture
	{
		unsigned int id;
		std::string type;
		aiString path;
	};

	class Material
	{
	public:
		std::vector<Texture> textures;
		Shader* shader;

		Material(Shader& shader)
		{
			this->shader = &shader;
		}

		void draw() const
		{
			if (!shader) return;

			shader->use();
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ������Ӧ������Ԫ
				// ��ȡ������ţ�diffuse_textureN �е� N��
				std::string number;
				std::string name = textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);

				// ���������λ��ֵ��Ȼ������󶨵����ֵ��
				shader->setInt(("material." + name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}

			// always good practice to set everything back to defaults once configured.
			glActiveTexture(GL_TEXTURE0);
		}
	};
}
