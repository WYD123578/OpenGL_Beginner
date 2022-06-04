#pragma once
#include <Plugin/imgui/imgui.h>
#include <Plugin/imgui/imgui_impl_glfw.h>
#include <Plugin/imgui/imgui_impl_opengl3.h>

#include "Light/DirectionLight.h"

class ImGuiWindow
{
public:
	bool show_demo_window = false;
	CameraControl& _camera;

	// float pitch = 0.0f;
	// float yaw = 0.0f;
	// glm::vec3 light_dir = glm::vec3(0.0, 0.0, 0.0);

	// 物体参数
	glm::vec3 ambient = glm::vec3(0.0, 0.1, 0.06);
	glm::vec3 diffuse = glm::vec3(0.0, 0.50980392, 0.50980392);
	glm::vec3 specular = glm::vec3(0.50196078, 0.50196078, 0.50196078);
	float shininess = .25 * 128.0;


	ImGuiWindow(GLFWwindow* window, CameraControl& camera): _camera(camera)
	{
		// 创建ImGui
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		// 设置输入模式
		//ImGuiIO& io = ImGui::GetIO();(void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// 初始化OpenGl3.0以及GLFW相关配置
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");
	}

	void updateWindow()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		drawCameraSettingWindow();
		drawObjectSettingWindow();

		for (int i = 0; i < _currentLightCount; i++)
		{
			//std::cout << &(_lights[i].pos) << std::endl;
			drawLightSettingWindow(_lights[i]);
		}
		
		// Rendering
		ImGui::Render();
	}

	bool addLight(Light* light)
	{
		if (_currentLightCount >= 8) return false;
		
		_lights[_currentLightCount++] = light;
		return true;
	}

	static void render()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

private:
	// 灯光参数
	Light* _lights[8];
	int _currentLightCount = 0;

	void drawCameraSettingWindow() const
	{
		ImGui::Begin("Camera Setting");
		ImGui::SliderFloat("camera pitch", &_camera.pitch, -89.0f, 89.0f);
		ImGui::SliderFloat("camera yaw", &_camera.yaw, -89.0f, 89.0f);
		ImGui::InputFloat3("camera pos", reinterpret_cast<float*>(&_camera.pos));
		_camera.updateCameraVectors();
		ImGui::End();
	}

	void drawLightSettingWindow(Light* light) const
	{
		ImGui::Begin("Light Setting");
		
		ImGui::SliderFloat3("light pos",reinterpret_cast<float*>(&(light->pos)), -5.0f, 5.0f);
		ImGui::ColorEdit3("light color", reinterpret_cast<float*>(&(light->color)));
		ImGui::ColorEdit3("light ambient", reinterpret_cast<float*>(&(light->ambient)));
		ImGui::ColorEdit3("light diffuse", reinterpret_cast<float*>(&(light->diffuse)));
		ImGui::ColorEdit3("light specular", reinterpret_cast<float*>(&(light->specular)));

		ImGui::End();
	}

	void drawObjectSettingWindow()
	{
		ImGui::Begin("Object Setting");

		ImGui::ColorEdit3("specular", reinterpret_cast<float*>(&specular));
		ImGui::SliderFloat("shininess", &shininess, 0.0f, 256.0f);

		ImGui::End();
	}

	// 平行光源方向计算
	/*void calcLightDir()
	{
		const float radiansXy = glm::radians(pitch);
		const float radiansXz = glm::radians(yaw);
		const float xzRadius = glm::cos(radiansXy);

		const float directionX = glm::sin(radiansXz) * xzRadius;
		const float directionY = glm::sin(radiansXy);
		const float directionZ = -glm::cos(radiansXz) * xzRadius;

		light_dir = glm::normalize(glm::vec3(directionX, directionY, directionZ));
	}*/
};
