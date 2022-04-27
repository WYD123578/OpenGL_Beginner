#pragma once
#include <Plugin/imgui/imgui.h>
#include <Plugin/imgui/imgui_impl_glfw.h>
#include <Plugin/imgui/imgui_impl_opengl3.h>

class ImGuiWindow
{
public:
	bool show_demo_window = false;

	CameraControl& _camera;

	float ambient_strength = 0.1f;
	float decay_rate = 0.1f;
	float specular_decay_rate = 32;

	// float pitch = 0.0f;
	// float yaw = 0.0f;
	// glm::vec3 light_dir = glm::vec3(0.0, 0.0, 0.0);

	glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 light_pos = glm::vec3(1.2f, 1.0f, 2.0f);
	glm::vec3 object_color = glm::vec3(0.45f, 0.55f, 0.60f);

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
		drawLightSettingWindow();

		// calcLightDir();

		// Rendering
		ImGui::Render();
	}

	static void render()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
private:
	void drawCameraSettingWindow() const
	{
		ImGui::Begin("Camera Setting");
		ImGui::SliderFloat("camera pitch", &_camera.pitch, -89.0f, 89.0f);
		ImGui::SliderFloat("camera yaw", &_camera.yaw, -89.0f, 89.0f);
		ImGui::InputFloat3("camera pos", reinterpret_cast<float*>(&_camera.pos));
		_camera.updateCameraVectors();
		ImGui::End();
	}

	void drawLightSettingWindow()
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Light Setting"); // Create a window called "Hello, world!" and append into it.
		
		ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
		ImGui::SliderFloat("ambient strength", &ambient_strength, 0.0f, 1.0f);
		ImGui::SliderFloat("decay rate", &decay_rate, 0.0f, 1.0f);
		ImGui::SliderFloat("specular decay rate", &specular_decay_rate, 0.0f, 256.0f);

		// ImGui::SliderFloat("light pitch", &pitch, -89.0f, 89.0f);
		// ImGui::SliderFloat("light yaw", &yaw, -89.0f, 89.0f);

		ImGui::SliderFloat3("light pos", reinterpret_cast<float*>(&light_pos), -5.0f, 5.0f);
		ImGui::ColorEdit3("light color", reinterpret_cast<float*>(&light_color));
		ImGui::ColorEdit3("object color", reinterpret_cast<float*>(&object_color));

		if (ImGui::Button("Button"))
			// Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
			ImGui::GetIO().Framerate);
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
