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
				glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
				// 获取纹理序号（diffuse_textureN 中的 N）
				std::string number;
				std::string name = textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);

				// 设置纹理的位置值，然后将纹理绑定到这个值上
				shader->setInt(("material." + name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}

			// always good practice to set everything back to defaults once configured.
			glActiveTexture(GL_TEXTURE0);
		}
	};
}
