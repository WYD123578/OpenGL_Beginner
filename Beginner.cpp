#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <iomanip>

#include "Plugin/stb_image.h"
#include <Tool/Shader.h>
#include <Tool/CameraControl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

GLFWwindow* window;
CameraControl camera(glm::vec3(0.0f, 0.0f, 3.0f));
int screenWidth, screenHeight;


void load_texture_from_resource(const string& pic_name, const GLint internal_format, const GLenum format)
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

bool is_first_mouse_pos = true;
float last_cursor_pos_x, last_cursor_pos_y;

void on_mouse_move_callback(GLFWwindow* win, const double x_pos_in, const double y_pos_in)
{
	const auto xPos = static_cast<float>(x_pos_in);
	const auto yPos = static_cast<float>(y_pos_in);

	if (is_first_mouse_pos)
	{
		last_cursor_pos_x = xPos;
		last_cursor_pos_y = yPos;
		is_first_mouse_pos = false;
	}

	const float xOffset = xPos - last_cursor_pos_x;
	const float yOffset = last_cursor_pos_y - yPos;

	/*cout << "=================================" << endl;
	cout << showpoint;
	cout << "lastX: " <<  setprecision(6) << lastCursorPosX << endl;
	cout << "curX: " << setprecision(6) << xPos << endl;
	cout << "offset: " << setprecision(6) << xOffset << endl;
	if (xOffset < 0.0f) cout << "<-" << endl;
	if (yOffset > 0.0f) cout << "->" << endl;
	cout << "=================================" << endl;
	cout << "" << endl*/
	;

	last_cursor_pos_x = xPos;
	last_cursor_pos_y = yPos;

	camera.onMouseMove(xOffset, yOffset);
}

void on_mouse_scroll_callback(GLFWwindow* win, const double xOffset, const double yOffset)
{
	camera.onMouseScroll(yOffset);
}

bool drawLine = false;

void processWindowKeyboardInput(float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		drawLine = !drawLine;
		drawLine ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.handleWindowKeyBoard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.handleWindowKeyBoard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.handleWindowKeyBoard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.handleWindowKeyBoard(RIGHT, deltaTime);
}

int main()
{
	if (!glfwInit())
	{
		cout << "初始化GLFW失败" << endl;
		return -1;
	}

	screenWidth = 800;
	screenHeight = 640;

	window = glfwCreateWindow(screenWidth, screenHeight, "Hello World", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// 注册监听事件
	glfwSetCursorPosCallback(window, on_mouse_move_callback);
	glfwSetScrollCallback(window, on_mouse_scroll_callback);

	// 设置窗口状态
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGL())
	{
		cout << "GLAD加载函数失败" << endl;
	}

	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	// 绘制时需要的操作对象
	unsigned int VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// 将数据写入顶点缓冲中
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
		load_texture_from_resource("container.jpg", GL_RGB, GL_RGB);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// 设置纹理边缘的重复规则
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// 设置纹理过滤
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		load_texture_from_resource("awesomeface.png", GL_RGBA, GL_RGBA);
	}

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	// MVP矩阵

	glm::mat4 model(1.0f), view(1.0f), projection(1.0f);
	{
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = camera.viewLookAtMat4();
		projection = glm::perspective(glm::radians(camera.fov()),
		                              static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 1.0f, 100.0f);
	}

	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	shader.setMatrix4("model", glm::value_ptr(model));
	shader.setMatrix4("view", glm::value_ptr(view));
	shader.setMatrix4("projection", glm::value_ptr(projection));

	float mixValue = 0.2f;
	shader.setFloat("mixValue", mixValue);

	// 渲染时间
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		// ---------------------
		// 清除屏幕
		glClearColor(0, 1, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const auto currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 更新Camera
		camera.updateCameraVectors();

		processWindowKeyboardInput(deltaTime);


		// 设置MVP
		{
			projection = glm::perspective(glm::radians(camera.fov()),
			                              static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 1.0f,
			                              100.0f);
			shader.setMatrix4("view", glm::value_ptr(camera.viewLookAtMat4()));
			shader.setMatrix4("projection", glm::value_ptr(projection));
		}

		glm::mat4 rot(1.0f);
		rot = glm::rotate(rot, currentFrame * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		shader.setMatrix4("rot", glm::value_ptr(rot));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);

		for (auto offset : cubePositions)
		{
			glm::mat4 pos(1.0f);
			pos = glm::translate(pos, offset);

			shader.setMatrix4("pos", glm::value_ptr(pos));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}
