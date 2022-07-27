#pragma once
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "material.h"
#include "Mesh.h"
#include "Plugin/stb_image.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

namespace Beginner
{
	class Model
	{
	public:
		/*  模型数据  */
		std::vector<Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string directory;
		bool gammaCorrection;

		/*  函数   */
		Model(const char* path)
		{
			loadModel(path);
		}
		
		// void draw(const Shader& shader) const
		// {
		// 	for (auto& mesh : meshes)
		// 	{
		// 		mesh.draw(shader);
		// 	}
		// }

	private:
		/*  函数   */
		void loadModel(const std::string& path)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
				return;
			}
			directory = path.substr(0, path.find_last_of('\\'));

			processNode(scene->mRootNode, scene);
		}

		void processNode(const aiNode* node, const aiScene* scene)
		{
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
				
			}

			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene);
			}
		}

		Beginner::Mesh processMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;

			// 转换顶点信息
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex{};

				const aiVector3D srcVertex = mesh->mVertices[i];
				const aiVector3D srcNormal = mesh->mNormals[i];
				vertex.Position = glm::vec3(srcVertex.x, srcVertex.y, srcVertex.z);
				vertex.Normal = glm::vec3(srcNormal.x, srcNormal.y, srcNormal.z);

				if (mesh->mTextureCoords[0]) // 网格里面每个顶点最多8个纹理坐标
				{
					const aiVector3D srcTexCoords = mesh->mTextureCoords[0][i];
					vertex.TexCoords = glm::vec2(srcTexCoords.x, srcTexCoords.y);
				}
				else
				{
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);
				}

				vertices.push_back(vertex);
			}

			// 转换索引
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				const aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}

			return {vertices, indices};
		}

		std::vector<Texture> loadMaterialTextures(int matIndex, const aiScene* scene)
		{
			// 转换材质
			std::vector<Texture> textures;
			aiMaterial* material = scene->mMaterials[matIndex];
			std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end()); // 插入另外一个结构的内容
			std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normals");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			return textures;
		}

		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
		{
			std::vector<Texture> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString path;
				mat->GetTexture(type, i, &path);

				bool skip = false;
				for (unsigned int j = 0; j < textures_loaded.size(); j++)
				{
					if (strcmp(textures_loaded[j].path.data, path.C_Str()) == 0)
					{
						textures_loaded.push_back(textures_loaded[j]);
						skip = true;
						break;
					}
				}
				if (!skip)
				{
					Texture texture;
					texture.id = TextureFromFile(path.C_Str(), directory);
					texture.type = typeName;
					texture.path = path;
					textures.push_back(texture);
					textures_loaded.push_back(texture);
				}
			}
			return textures;
		}

		unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false)
		{
			auto filename = std::string(path);
			filename = directory + '/' + filename;

			unsigned int textureID;
			glGenTextures(1, &textureID);

			int width, height, nrComponents;
			unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
			if (data)
			{
				GLenum format = GL_RGB;
				if (nrComponents == 1)
					format = GL_RED;
				else if (nrComponents == 3)
					format = GL_RGB;
				else if (nrComponents == 4)
					format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else
			{
				std::cout << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(data);
			}

			return textureID;
		}
	};
}