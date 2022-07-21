#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <iomanip>

#include "Tool/CameraControl.h"
#include "Tool/ImGuiWindow.h"
#include "Mesh/Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;

GLFWwindow* window;
CameraControl camera(glm::vec3(-2.5f, 2.8f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -25.0f, 35.0f);
int screenWidth, screenHeight;

void on_framebuffer_size_callback(GLFWwindow* _window, int _width, int _height)
{
	glViewport(0, 0, _width, _height);
}

unsigned int load_texture_from_resource(const string& pic_name)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	string path = "Resource/" + pic_name;

	stbi_set_flip_vertically_on_load(true);

	// 加载图片
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		// 绑定并输入纹理
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		cout << "there is no data of picture" << endl;
	}
	stbi_set_flip_vertically_on_load(false);
	stbi_image_free(data);

	return textureID;
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

	last_cursor_pos_x = xPos;
	last_cursor_pos_y = yPos;

	camera.processMouseMovement(xOffset, yOffset);
}

void on_mouse_scroll_callback(GLFWwindow* win, const double xOffset, const double yOffset)
{
	camera.processMouseScroll(yOffset);
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

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_KEY_DOWN)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void error_callback(int error, const char* msg)
{
	std::string s;
	s = " [" + std::to_string(error) + "] " + msg + '\n';
	std::cerr << s << std::endl;
}

Light* lightsInScene = new Light[8]();

int main()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		cout << "初始化GLFW失败" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	glfwSetFramebufferSizeCallback(window, on_framebuffer_size_callback);
	glfwSetCursorPosCallback(window, on_mouse_move_callback);
	glfwSetScrollCallback(window, on_mouse_scroll_callback);

	// 设置窗口状态
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGL())
	{
		cout << "GLAD加载函数失败" << endl;
	}

	// 创建imGui窗口
	ImGuiWindow imGuiWin(window, camera);

	// 准备ShaderProgram
	Shader objectShader("Shader/StandardLightShader.vs", "Shader/StandardLightShader.fs");
	if (objectShader.ID == 0)
	{
		cout << "there is no shader program" << endl;
		return -1;
	}

	Shader outlineShader("Shader/Outline.vs", "Shader/Outline.fs");
	if (outlineShader.ID == 0)
	{
		cout << "there is no shader program" << endl;
		return -1;
	}

	// 渲染时间
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// 添加光照
	DirectionLight dirLight = DirectionLight();
	dirLight.pos = glm::vec3(1, 0, 0);
	imGuiWin.addLight(&dirLight);

	// 创建模型
	Model drawTargetModel = Model("F:\\OpenGL\\Projects\\Beginner\\Resource\\nanosuit\\nanosuit.obj");

	vector<Vertex> plane_vertices = vector<Vertex>();
	plane_vertices.push_back(Vertex{ glm::vec3(0.5, 0.5, 0), glm::vec3(0,0,1), glm::vec2(1,1) });
	plane_vertices.push_back(Vertex{ glm::vec3(0.5, -0.5, 0), glm::vec3(0,0,1), glm::vec2(1,0) });
	plane_vertices.push_back(Vertex{ glm::vec3(-0.5, 0.5, 0), glm::vec3(0,0,1), glm::vec2(0,1) });
	plane_vertices.push_back(Vertex{ glm::vec3(-0.5, -0.5, 0), glm::vec3(0,0,1), glm::vec2(0,0) });

	vector<unsigned int> plane_indices = vector<unsigned int>();
	plane_indices.push_back(0);
	plane_indices.push_back(1);
	plane_indices.push_back(2);
	plane_indices.push_back(1);
	plane_indices.push_back(2);
	plane_indices.push_back(3);

	Mesh plane = Mesh(plane_vertices, plane_indices, vector<Texture>());

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	while (!glfwWindowShouldClose(window))
	{
		// ---------------------
		imGuiWin.updateWindow();

		// 清除屏幕
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		const auto currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 更新Camera
		processWindowKeyboardInput(deltaTime);

		// 绘制物体
		glm::mat4 model(1.0f), view(1.0f), projection(1.0f);
		view = camera.viewLookAtMat4();
		projection = glm::perspective(glm::radians(camera.fov()),
		                              static_cast<float>(screenWidth) / static_cast<float>(screenHeight), camera.near,
		                              camera.far);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		objectShader.use();
		{
			objectShader.setMatrix4("view", glm::value_ptr(view));
			objectShader.setMatrix4("projection", glm::value_ptr(projection));

			objectShader.setFloat("material.shininess", imGuiWin.shininess);

			objectShader.setVec3("dirLight.color", dirLight.color);
			objectShader.setVec3("dirLight.position", dirLight.pos);

			objectShader.setVec3("dirLight.ambient", dirLight.ambient);
			objectShader.setVec3("dirLight.diffuse", dirLight.diffuse);
			objectShader.setVec3("dirLight.specular", dirLight.specular);

			objectShader.setVec3("dirLight.direction", dirLight.pos);
			objectShader.setFloat("light.cutOff", glm::cos(glm::radians(12.0f)));
			objectShader.setFloat("light.outerCutOff", glm::cos(glm::radians(24.0f)));

			objectShader.setVec3("worldCameraPos", imGuiWin._camera.pos);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(50.0f, -10.0f, 0.0f));
			objectShader.setMatrix4("model", glm::value_ptr(model));

			// DrawCall
			drawTargetModel.draw(objectShader);
		}

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glDisable(GL_DEPTH_TEST);
		glStencilMask(0x00);
		outlineShader.use();
		{
			outlineShader.setMatrix4("view", glm::value_ptr(view));
			outlineShader.setMatrix4("projection", glm::value_ptr(projection));
			outlineShader.setMatrix4("model", glm::value_ptr(model));

			drawTargetModel.draw(outlineShader);
		}
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);



		// 放在最后绘制窗口
		ImGuiWindow::render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// 删除Shader Program
	glDeleteProgram(objectShader.ID);
	glDeleteProgram(outlineShader.ID);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
