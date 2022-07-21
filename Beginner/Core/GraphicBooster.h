#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Graphic 
{
public:
	Graphic() {};

	void OnStart();
	void OnUpdate();
	void OnDestroy();
};

class GraphicBooster
{
private:
	Graphic* _runningGraphic;
	GLFWwindow* _window;

	void UpdateGraphic();

public:
	void InitGraphic(Graphic* graphic);

	void StartGraphic();
	void DestroyGraphic();
};