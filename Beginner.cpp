#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "Plugin/stb_image.h"
#include <Tool/Shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

GLFWwindow* window;

void loadTextureFromResource(const string& pic_name, const GLint internal_format, const GLenum format)
{
	string path = "Resource/" + pic_name;

	stbi_set_flip_vertically_on_load(true);
	// 加载图片
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		// 绑定并输入纹理
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "there is no data of picture" << endl;
	}
	stbi_set_flip_vertically_on_load(false);

	stbi_image_free(data);
}

int main()
{
	if (!glfwInit())
	{
		cout << "初始化GLFW失败" << endl;
		return -1;
	}

	window = glfwCreateWindow(640, 640, "Hello World", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		cout << "GLAD加载函数失败" << endl;
	}

	// 定义要绘制的顶点信息
	constexpr float verticals[] =
	{
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 左上
	};

	const unsigned int indices[] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	// 绘制时需要的操作对象
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// 将数据写入顶点缓冲中
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticals), verticals, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// 准备ShaderProgram
	Shader shader("Shader/Texture.vs", "Shader/Texture.fs");
	if (shader.ID == 0)
	{
		cout << "there is no shader program" << endl;
		return -1;
	}

	// 生成纹理
	unsigned int texture, texture2;
	{
		glGenTextures(1, &texture);
		glGenTextures(1, &texture2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// 设置纹理边缘的重复规则
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		// 设置纹理过滤
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 加载图片
		loadTextureFromResource("container.jpg", GL_RGB, GL_RGB);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// 设置纹理边缘的重复规则
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// 设置纹理过滤
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		loadTextureFromResource("awesomeface.png", GL_RGBA, GL_RGBA);
	}

	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	float mixValue = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		// ---------------------
		// 清除屏幕
		glClearColor(0, 1, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// 设置绘制线框
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			mixValue += 0.01f;
			if (mixValue >= 1.0f)
			{
				mixValue = 1.0f;
			}
			shader.setFloat("mixValue", mixValue);
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			mixValue -= 0.01f;
			if (mixValue <= 0.0f)
			{
				mixValue = 0.0f;
			}
			shader.setFloat("mixValue", mixValue);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);

		// 旋转
		{
			glm::mat4 rot(1.0f);
			rot = glm::rotate(rot, static_cast<float>(glfwGetTime()), glm::vec3(0.0, 0.0, 1.0));
			shader.setMatrix4("transform", glm::value_ptr(rot));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		// 第二个
		{
			glm::mat4 trans(1.0f);
			trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
			const auto scale = static_cast<float>(glm::sin(glfwGetTime()))+1;
			trans = glm::scale(trans, glm::vec3(scale, scale, scale));

			shader.setMatrix4("transform", glm::value_ptr(trans));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}
