#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <iomanip>
#include <map>
#include <chrono>

#include "Tool/CameraControl.h"
#include "Tool/ImGuiWindow.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Render/render_pass.h"
#include "Mesh/basic_mesh.h"
#include "Mesh/Mesh.h"
#include "Plugin/stb_image.h"

using namespace std;

GLFWwindow* window;
CameraControl camera(glm::vec3(-2.5f, 2.8f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -25.0f, 35.0f);
float screenWidth, screenHeight;

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
		if (nrChannels == 4)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
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


std::chrono::steady_clock::time_point m_lastTimePoint{std::chrono::steady_clock::now()};

float getDeltaTime()
{
	float deltaTime = 0;
	{
		using namespace std::chrono;

		steady_clock::time_point tickTimePoint = steady_clock::now();
		duration<float> timeSpan = duration_cast<duration<float>>(tickTimePoint - m_lastTimePoint);
		deltaTime = timeSpan.count();
		m_lastTimePoint = tickTimePoint;
	}
	return deltaTime;
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
	Shader objectShader("Beginner/Shader/StandardLightShader.vert", "Beginner/Shader/StandardLightShader.frag");
	if (objectShader.ID == 0)
	{
		cout << "there is no shader program" << endl;
		return -1;
	}

	Shader grassShader("Beginner/Shader/Grass.vert", "Beginner/Shader/Grass.frag");
	if (grassShader.ID == 0)
	{
		cout << "there is no shader program" << endl;
		return -1;
	}

	vector<Shader> shaderArray
	{
		Shader("Beginner/Shader/StandardLightShader.vert", "Beginner/Shader/StandardLightShader.frag"),
		Shader("Beginner/Shader/Grass.vert", "Beginner/Shader/Grass.frag"),
		Shader("Beginner/Shader/GlScreen.vert", "Beginner/Shader/GlScreen.frag"),
	};

	for (Shader s : shaderArray)
	{
		if (s.ID == 0)
		{
			cout << "there is no shader program" << endl;
			return -1;
		}
	}

	// 添加光照
	DirectionLight dirLight = DirectionLight();
	dirLight.pos = glm::vec3(1, 0, 0);
	imGuiWin.addLight(&dirLight);

	// 创建模型
	// Model drawTargetModel = Model("F:\\OpenGL\\Projects\\Beginner\\Resource\\nanosuit\\nanosuit.obj");
	Beginner::Mesh plane = {Beginner::SQUARE_VERTEX, Beginner::SQUARE_INDICES};
	Beginner::Mesh floor = {Beginner::PLANE_VERTEX, Beginner::PLANE_INDICES};

	int grassTexture = load_texture_from_resource("blending_transparent_window.png");

	Beginner::RenderPass globalPass(Beginner::RenderPassParam{ true, true, true, true, true });
	globalPass.setRenderState();

	while (!glfwWindowShouldClose(window))
	{
		// ---------------------
		imGuiWin.updateWindow();

		// 清除屏幕
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		const auto currentFrame = static_cast<float>(glfwGetTime());
		float deltaTime = getDeltaTime();

		// 更新Camera
		processWindowKeyboardInput(deltaTime);

		// 绘制物体
		glm::mat4 model(1.0f);


		Beginner::RenderPass::setShaderMVPParam(&objectShader, camera, screenWidth, screenHeight);
		Beginner::RenderPass::setShaderMVPParam(&grassShader, camera, screenWidth, screenHeight);
		Beginner::RenderPass::setShaderLightParam(&objectShader, &dirLight);

		objectShader.use();
		{
			objectShader.setFloat("material.shininess", imGuiWin.shininess);
			
			model = glm::mat4(1.0f);
			objectShader.setMatrix4("model", glm::value_ptr(model));

			// DrawCall
			floor.draw(objectShader);
		}

		grassShader.use();
		{
			glBindTexture(GL_TEXTURE_2D, grassTexture);
		}

		map<float, glm::vec3> sorted;
		for (char i = 0; i < 10; i++)
		{
			glm::vec3 planePos = glm::vec3(0, 0, i * 0.6);
			float distance = glm::length(camera.pos - planePos);
			sorted[distance] = planePos;
		}

		for (auto it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1);
			model = glm::translate(model, it->second);
			grassShader.setMatrix4("model", glm::value_ptr(model));
			plane.draw(grassShader);
		}

		// 取消绑定对其他Pass造成的影响
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

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
	glDeleteProgram(grassShader.ID);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
