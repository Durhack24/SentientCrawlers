#pragma once
#include <memory>

#include "Interface.h"

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

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
	std::unique_ptr<Interface> interface;
	int w, h;
};