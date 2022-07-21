#include "GraphicBooster.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void Graphic::OnStart()
{
	cout << "graphic started" << endl;
}

void Graphic::OnUpdate()
{
	cout << "graphic update" << endl;
}

void Graphic::OnDestroy()
{
	cout << "graphic destroied" << endl;
}

void GraphicBooster::InitGraphic(Graphic* graphic)
{
	cout << "boost graphic init" << endl;

	if (!glfwInit())
	{
		cout << "³õÊ¼»¯GLFWÊ§°Ü" << endl;
		return;
	}

	_runningGraphic = graphic;
	_window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);

	if (!_window) {
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_window);

	if (!gladLoadGL()) {
		cout << "GLAD¼ÓÔØº¯ÊýÊ§°Ü" << endl;
	}

	StartGraphic();

	while (!glfwWindowShouldClose(_window)) 
	{
		UpdateGraphic();
		if (glfwGetKey(_window, GLFW_KEY_ENTER) == GLFW_PRESS)
		{

		}

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	glfwTerminate();
	DestroyGraphic();
}

void GraphicBooster::StartGraphic()
{
	cout << "boost graphic start" << endl;
	_runningGraphic->OnStart();
}

void GraphicBooster::UpdateGraphic()
{
	cout << "boost graphic update" << endl;
	_runningGraphic->OnUpdate();
}

void GraphicBooster::DestroyGraphic()
{
	cout << "boost graphic destroy" << endl;
	_runningGraphic->OnDestroy();
}
