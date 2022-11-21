#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <iomanip>
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
#include "Tool/resource_manager.h"

using namespace std;

GLFWwindow* window;
CameraControl camera(glm::vec3(-2.5f, 2.8f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -25.0f, 35.0f);
int screenWidth, screenHeight;

void on_framebuffer_size_callback(GLFWwindow* _window, int _width, int _height)
{
	glViewport(0, 0, _width, _height);
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
	vector<Shader*> shaderArray
	{
		new Shader("Beginner/Shader/StandardLightShader.vert", "Beginner/Shader/StandardLightShader.frag"),
		new Shader("Beginner/Shader/Grass.vert", "Beginner/Shader/Grass.frag"),
		new Shader("Beginner/Shader/GlScreen.vert", "Beginner/Shader/GlScreen.frag"),
		new Shader("Beginner/Shader/skybox.vert", "Beginner/Shader/skybox.frag"),
		new Shader("Beginner/Shader/reflect.vert", "Beginner/Shader/reflect.frag"),
		new Shader("Beginner/Shader/refract.vert", "Beginner/Shader/refract.frag"),
		new Shader("Beginner/Shader/gl_param.vert", "Beginner/Shader/gl_param.frag"),
	};

	for (const Shader* s : shaderArray)
	{
		if (s->ID == 0)
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
	// Beginner::Model drawTargetModel = Beginner::Model("F:\\OpenGL\\Projects\\Beginner\\Resource\\nanosuit\\nanosuit.obj");
	Beginner::Mesh plane = {Beginner::SQUARE_VERTEX, Beginner::SQUARE_INDICES};
	Beginner::Mesh floor = {Beginner::PLANE_VERTEX, Beginner::PLANE_INDICES};
	Beginner::Mesh quad = {Beginner::QUAD_VERTICES_ARC, 2, false, true};
	Beginner::Mesh cube = {Beginner::CUBE_VERTEX, 3, true, true};
	Beginner::Mesh skybox = { Beginner::SKYBOX_VERTICES, 3, false, false };

	vector<std::string> faces
	{
		"Resource/skybox/right.jpg",
		"Resource/skybox/left.jpg",
		"Resource/skybox/top.jpg",
		"Resource/skybox/bottom.jpg",
		"Resource/skybox/front.jpg",
		"Resource/skybox/back.jpg"
	};

	unsigned int grassTexture = ResourceLoader::loadTexture("container.jpg");
	unsigned int cubeMapTexture = ResourceLoader::loadCubeMap(faces);

	Beginner::RenderPass globalPass(Beginner::RenderPassParam{true, true, false, false, false});
	globalPass.setRenderState();

	// 帧缓冲
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// 纹理附件
	GLuint textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 渲染缓冲附件
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// 绑定渲染帧缓冲，并检查其完整性
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[Error Frame Buffer] Frame Buffer is not complete\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // 检查完生成的FBO正确，就回到默认状态

	// 深度缓冲;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glEnable(GL_PROGRAM_POINT_SIZE);

	while (!glfwWindowShouldClose(window))
	{
		// ---------------------
		imGuiWin.updateWindow();
		const auto currentFrame = static_cast<float>(glfwGetTime());
		float deltaTime = getDeltaTime();

		// 更新Camera
		processWindowKeyboardInput(deltaTime);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		// 清除屏幕
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// 绘制操作
		{
			glm::mat4 model(1.0f);

			for (const Shader* s : shaderArray)
			{
				Beginner::RenderPass::setShaderMVPParam(s, camera, screenWidth, screenHeight);
				Beginner::RenderPass::setShaderLightParam(s, &dirLight);
			}

			// shaderArray[1]->use();
			// {
			// 	glBindTexture(GL_TEXTURE_2D, grassTexture);
			//
			// 	map<float, glm::vec3> sorted;
			// 	for (char i = 0; i < 1; i++)
			// 	{
			// 		glm::vec3 planePos = glm::vec3(0, 0, i * 0.6);
			// 		float distance = glm::length(camera.pos - planePos);
			// 		sorted[distance] = planePos;
			// 	}
			//
			// 	for (auto it = sorted.rbegin(); it != sorted.rend(); ++it)
			// 	{
			// 		model = glm::mat4(1);
			// 		model = glm::translate(model, it->second);
			// 		shaderArray[1]->setMatrix4("model", glm::value_ptr(model));
			// 		cube.draw(*shaderArray[1]);
			// 	}
			//
			// 	// 取消绑定对其他Pass造成的影响
			// 	glActiveTexture(GL_TEXTURE0);
			// 	glBindTexture(GL_TEXTURE_2D, 0);
			// }

			shaderArray[5]->use();
			{
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(1, 0, 0));

				glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
				shaderArray[5]->setMatrix4("model", glm::value_ptr(model));
				cube.draw(*shaderArray[5]);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}

			shaderArray[6]->use();
			{
				model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(3, 0, 0));

				glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
				shaderArray[6]->setMatrix4("model", glm::value_ptr(model));
				cube.draw(*shaderArray[6]);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}

			shaderArray[3]->use();
			{
				glDepthFunc(GL_LEQUAL);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
				glm::mat4 view = glm::mat4(glm::mat3(camera.viewLookAtMat4()));
				shaderArray[3]->setMatrix4("view", glm::value_ptr(view));
				skybox.draw(*shaderArray[3]);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				glDepthFunc(GL_LESS);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		shaderArray[2]->use();
		{
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			quad.draw(*shaderArray[2]);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
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
	for (Shader* s : shaderArray)
	{
		glDeleteProgram(s->ID);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
