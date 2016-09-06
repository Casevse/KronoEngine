#pragma once

#include <GLFW\glfw3.h>
#include <Resource\ResourceManager.h>
#include <Scene\Scene.h>

class Renderer {
public:

	int					Init(int width, int height, bool fullscreen, const char *title);
	void				SetTitle(const char *title);
	void				Close();

	void				BeginFrame();
	void				Draw();
	void				Clear();
	void				ClearResources();

	bool				IsRunning();

	void				SetLightsVisible(const bool visible);
	void				SetGlobalLightParameters(const glm::vec3 &position, const float intensity, const glm::ivec4 &color, const float offset);

private:

	GLFWwindow			*mWindow;
	

};