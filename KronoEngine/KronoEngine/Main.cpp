#include <Config.h>
#include <Renderer.h>

Renderer renderer;

void ConfigureScene() {
	renderer.SetGlobalLightParameters(glm::vec3(-100.0f, 100.0f, -100.0f), 0.8f, glm::ivec4(255, 255, 255, 255), 100.0f);

	renderer.AddCamera(glm::vec3(0.0f, 2.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	renderer.AddCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::ivec4(255, 255, 0, 255));
	renderer.AddLight(glm::vec3(2.0f, 2.0f, 2.0f), glm::ivec4(0, 255, 0, 255), 0.5f , 0.5f, 0.5f);
	
	Node *mesh = renderer.AddMesh("../Media/Meshes/Kedi/kedi-static.km", glm::vec3(2.0f, 0.5f, 0.0f));
	mesh->SetTexture("../Media/Textures/Kedi/kedi-texture.png");

	Node *plane = renderer.AddPlane(glm::vec3(0.0f, -0.5f, 0.0f), 8, 8);
	plane->SetColor(glm::ivec4(0, 100, 100, 255));

	renderer.AddSphere(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::ivec4(0, 255, 255, 255));
}

int main(void) {
	renderer.Init(Config::width, Config::height, Config::fullscreen, "KronoEngine");

	ConfigureScene();

	while (renderer.IsRunning()) {
		renderer.BeginFrame();
		renderer.Draw();
	}

	renderer.Close();

	return 0;
}