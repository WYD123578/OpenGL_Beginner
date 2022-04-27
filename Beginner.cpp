#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <iomanip>

#include <Plugin/stb_image.h>
#include <Tool/Shader.h>
#include <Tool/CameraControl.h>
#include <Tool/ImGuiWindow.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



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

	// 创建imGui窗口
	ImGuiWindow imGuiWin(window, camera);

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
	Shader objectShader("Shader/ObjectShader.vs", "Shader/ObjectShader.fs");
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

	while (!glfwWindowShouldClose(window))
	{
		// ---------------------
		imGuiWin.updateWindow();

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

		objectShader.use();
		{
			// 设置物体Shader
			glm::mat4 model(1.0f);

			objectShader.setVec3("objectColor", imGuiWin.object_color);
			objectShader.setVec3("lightColor", imGuiWin.light_color);
			objectShader.setVec3("lightPos", imGuiWin.light_pos);
			objectShader.setVec3("viewPos", camera.pos);
			//objectShader.setVec3("lightDir", imGuiWin.light_dir);

			objectShader.setFloat("ambientStrength", imGuiWin.ambient_strength);
			objectShader.setFloat("decayRate", imGuiWin.decay_rate);
			objectShader.setFloat("specularDecayRate", imGuiWin.specular_decay_rate);

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
			model = glm::translate(model, imGuiWin.light_pos);
			model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

			lightShader.setMatrix4("model", glm::value_ptr(model));
			lightShader.setMatrix4("view", glm::value_ptr(view));
			lightShader.setMatrix4("projection", glm::value_ptr(projection));
			
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// 放在最后绘制窗口
		ImGuiWindow::render();

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
