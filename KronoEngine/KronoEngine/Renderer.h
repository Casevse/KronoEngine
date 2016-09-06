#pragma once

#include <GLFW\glfw3.h>
#include <Resource\ResourceManager.h>
#include <Scene\Scene.h>
#include <Node.h>

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

	Node				*AddVoid(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &rotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f), Node *parent = NULL);
	Node				*AddCube(const glm::vec3 &position, const glm::vec3 &scale, const glm::ivec4 &color, Node *parent = NULL);
	Node				*AddSphere(const glm::vec3 &position, const glm::vec3 &scale, const glm::ivec4 &color, Node *parent = NULL);
	Node				*AddCamera(const glm::vec3 &position, const glm::vec3 &target, Node *parent = NULL);
	Node				*AddMesh(char *path, const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &rotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f), const bool isStatic = false, Node *parent = NULL);
	Node				*AddLight(const glm::vec3 &position, const glm::ivec4 &color = glm::ivec4(255, 255, 255, 255), const float ambientIntensity = 0.7f, const float diffuseIntensity = 0.7f, const float attenuation = 0.3f, Node *parent = NULL);
	Node				*AddPlane(const glm::vec3 &position, const int sizeX = 1, const int sizeZ = 1, Node *parent = NULL);
	Node				*AddParticle(const glm::vec3 &position, const float size, const float lifetime, const glm::vec2 &velocityRange, const glm::vec3 &acceleration, const int numberOfParticles, const float width,
								const float dispersion, char *path, const glm::ivec4 &color = glm::ivec4(255, 255, 255, 255), Node *parent = NULL);

	void				SetActiveCamera(Node *camera);
	void				SetLightsVisible(const bool visible);
	void				SetGlobalLightParameters(const glm::vec3 &position, const float intensity, const glm::ivec4 &color, const float offset);

private:

	GLFWwindow			*mWindow;
	
	Scene				*mScene;
	ProgramManager		*mProgramManager;
	ResourceManager		*mResourceManager;

};