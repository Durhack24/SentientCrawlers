#pragma once
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include "Interface.h"

class App
{
public:
	bool Initialize();
	void MainLoop();

	~App();

protected:
	static void StartFrame();
	void UpdateViewport();
	static void RenderImgui();

	GLFWwindow* window;
	Interface interface {};
	int w, h;
};