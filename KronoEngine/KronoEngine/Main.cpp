#include <Config.h>
#include <Renderer.h>

Renderer renderer;
const double kFps = 60.0;
const double kDeltaTime = 1.0 / kFps;
double accumulator = 0.0;
double frameStart = 0.0;
double currentTime = 0.0;

Node *camera = NULL;
Node *cube = NULL;
Node *light = NULL;
Node *mesh = NULL;
Node *plane = NULL;
Node *sphere = NULL;
Node *particle = NULL;

float angle = 0.0f;

void ConfigureScene() {
	renderer.SetGlobalLightParameters(glm::vec3(-100.0f, 100.0f, -100.0f), 0.8f, glm::ivec4(255, 255, 255, 255), 100.0f);

	camera = renderer.AddCamera(glm::vec3(0.0f, 2.0f, -4.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	cube = renderer.AddCube(glm::vec3(2.0f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::ivec4(255, 255, 255, 255));
	cube->SetTexture("../Media/Textures/Test/Dice.png");
	
	light = renderer.AddLight(glm::vec3(-2.0f, 1.0f, 2.0f), glm::ivec4(0, 255, 0, 255), 0.5f , 0.5f, 0.5f);
	
	mesh = renderer.AddMesh("../Media/Meshes/Kedi/kedi-static.km", glm::vec3(0.0f, 0.5f, 0.0f));
	mesh->SetTexture("../Media/Textures/Kedi/kedi-texture.png");

	plane = renderer.AddPlane(glm::vec3(0.0f, -0.5f, 0.0f), 8, 8);
	plane->SetColor(glm::ivec4(0, 100, 100, 255));

	sphere = renderer.AddSphere(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::ivec4(255, 100, 0, 255));

	particle = renderer.AddParticle(glm::vec3(-2.0f, 0.0f, 0.0f), 2048.0f, 2.0f, glm::vec2(0.7f, 1.2f), glm::vec3(0.0f, -0.1f, 0.0f), 200, 0.8f, 0.0f, "../Media/Textures/Particle/FireParticle.png", glm::ivec4(255, 200, 0, 255));
}

void UpdateScene() {
	float speed = (3.14f * 2.0f / 6.0f);
	float radius = 0.1f;

	angle += kDeltaTime * speed;

	camera->SetPosition(glm::vec3(glm::cos(angle) * radius, 0.0f, glm::sin(angle) * radius));

	cube->SetRotation(cube->GetRotation() + glm::vec3(2.0f, 2.0f, 2.0f));
}

void MainLoop() {
	currentTime = glfwGetTime();
	accumulator += (currentTime - frameStart);
	frameStart = currentTime;

	if (accumulator > 0.2) {
		accumulator = 0.2;
	}

	renderer.BeginFrame();
	while (accumulator > kDeltaTime) {
		accumulator -= kDeltaTime;
		UpdateScene();
	}
	renderer.Draw();
}

int main(void) {
	renderer.Init(Config::width, Config::height, Config::fullscreen, "KronoEngine");

	ConfigureScene();

	while (renderer.IsRunning()) {
		MainLoop();
	}

	renderer.Close();

	return 0;
}