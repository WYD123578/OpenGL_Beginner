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

#include <Plugin/imgui/imgui.h>
#include <Plugin/imgui/imgui_impl_glfw.h>
#include <Plugin/imgui/imgui_impl_opengl3.h>

using namespace std;

GLFWwindow* window;
CameraControl camera(glm::vec3(-2.5f, 2.8f, 5.0f), glm::vec3(0.0f,1.0f,0.0f), -25.0f, 35.0f);
int screenWidth, screenHeight;

void on_framebuffer_size_callback(GLFWwindow* _window, int _width, int _height)
{
	glViewport(0, 0, _width, _height);
}

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
}

int main()
{
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
	//glfwSetCursorPosCallback(window, on_mouse_move_callback);
	//glfwSetScrollCallback(window, on_mouse_scroll_callback);

	// 设置窗口状态
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGL())
	{
		cout << "GLAD加载函数失败" << endl;
	}

	// ===================================================
	//IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// ===================================================

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// 绘制时需要的操作对象
	unsigned int VAO, VBO, lightVAO;

	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	// 将以下对顶点的设置记录到VAO里面
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
	}

	glBindVertexArray(lightVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	// 准备ShaderProgram
	Shader objectShader("Shader/Texture.vs", "Shader/Texture.fs");
	if (objectShader.ID == 0)
	{
		cout << "there is no shader program" << endl;
		return -1;
	}

	Shader lightShader("Shader/LampShader.vs", "Shader/LampShader.fs");
	if (lightShader.ID == 0)
	{
		cout << "there is no shader program" << endl;
		return -1;
	}


	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	// 渲染时间
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// 定义光照颜色
	constexpr glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);


	while (!glfwWindowShouldClose(window))
	{
		// ---------------------
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();

		// 清除屏幕
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const auto currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 更新Camera
		processWindowKeyboardInput(deltaTime);

		// 绘制物体
		glm::mat4 view(1.0f), projection(1.0f);
		view = camera.viewLookAtMat4();
		projection = glm::perspective(glm::radians(camera.fov()),
			static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 1.0f,
			100.0f);

		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

		objectShader.use();
		{
			// 设置物体Shader
			glm::mat4 model(1.0f);

			objectShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
			objectShader.setVec3("lightColor", lightColor);
			objectShader.setVec3("lightPos", lightPos);

			objectShader.setMatrix4("model", glm::value_ptr(model));
			objectShader.setMatrix4("view", glm::value_ptr(view));
			objectShader.setMatrix4("projection", glm::value_ptr(projection));

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// 绘制光源
		lightShader.use();
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

			lightShader.setMatrix4("model", glm::value_ptr(model));
			lightShader.setMatrix4("view", glm::value_ptr(view));
			lightShader.setMatrix4("projection", glm::value_ptr(projection));
			
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// 放在最后绘制窗口
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
