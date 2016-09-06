#include <Renderer.h>

int Renderer::Init(int width, int height, bool fullscreen, const char *title) {
	if (!glfwInit()) {
		return -1;
	}

	if (fullscreen) {
		mWindow = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
	} else {
		mWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	}

	if (!mWindow) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(mWindow);

	GLenum error = glewInit();

	if (error != GLEW_OK) {
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Back Face Culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Allow to change the Point Sprite size from inside the shader.
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	mProgramManager = new ProgramManager();
	mProgramManager->InitPrograms();

	mResourceManager = new ResourceManager(mProgramManager);

	// Create scene tree.
	mScene = new Scene(mProgramManager, mResourceManager);

	return 1;
}

void Renderer::SetTitle(const char *title) {
	glfwSetWindowTitle(mWindow, title);
}

void Renderer::Close() {
	glfwDestroyWindow(mWindow);
	delete mScene;
	delete mResourceManager;
	delete mProgramManager;
}

void Renderer::BeginFrame() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw() {
	mScene->Draw();

	glfwSwapBuffers(mWindow);

	// TODO
	glfwPollEvents();
}

void Renderer::Clear() {
	mScene->Clear();
}

void Renderer::ClearResources() {
	mResourceManager->Clear();
}

bool Renderer::IsRunning() {
	return !glfwWindowShouldClose(mWindow);
}

Node *Renderer::AddVoid(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale, Node *parent) {
	SceneNode *sceneNodeParent = NULL;

	if (parent) {
		sceneNodeParent = parent->GetSceneNode();
	}

	TransformEntity *transform = mScene->CreateTransform(position, rotation, scale);
	SceneNode *sceneNode = mScene->CreateNode(transform, sceneNodeParent);
	Node *node = new Node(sceneNode);

	return node;
}

Node *Renderer::AddCube(const glm::vec3 &position, const glm::vec3 &scale, const glm::ivec4 &color, Node *parent) {
	SceneNode *sceneNodeParent = NULL;

	if (parent) {
		sceneNodeParent = parent->GetSceneNode();
	}

	TransformEntity *transform = mScene->CreateTransform(position, glm::vec3(0.0f, 0.0f, 0.0f), scale);
	SceneNode *transformNode = NULL;

	if (sceneNodeParent) {
		transformNode = mScene->CreateNode(transform, sceneNodeParent);
	} else {
		transformNode = mScene->CreateNode(transform, NULL);
	}

	MeshEntity *cube = mScene->CreateMesh("../Media/Meshes/Primitives/cube.km");
	SceneNode *sceneNode = mScene->CreateNode(cube, transformNode);

	cube->SetColor(color);

	Node *node = new Node(sceneNode);

	return node;
}

Node *Renderer::AddSphere(const glm::vec3 &position, const glm::vec3 &scale, const glm::ivec4 &color, Node *parent) {
	SceneNode *sceneNodeParent = NULL;

	if (parent) {
		sceneNodeParent = parent->GetSceneNode();
	}

	TransformEntity *transform = mScene->CreateTransform(position, glm::vec3(0.0f, 0.0f, 0.0f), scale);
	SceneNode *transformNode = NULL;

	if (sceneNodeParent) {
		transformNode = mScene->CreateNode(transform, sceneNodeParent);
	} else {
		transformNode = mScene->CreateNode(transform, NULL);
	}

	MeshEntity *sphere = mScene->CreateMesh("../Media/Meshes/Primitives/sphere.km");
	SceneNode *sceneNode = mScene->CreateNode(sphere, transformNode);

	sphere->SetColor(color);

	Node *node = new Node(sceneNode);

	return node;
}

Node *Renderer::AddCamera(const glm::vec3 &position, const glm::vec3 &target, Node *parent) {
	SceneNode *sceneNodeParent = NULL;

	if (parent) {
		sceneNodeParent = parent->GetSceneNode();
	}

	TransformEntity *transform = mScene->CreateTransform(position);
	SceneNode *transformNode = NULL;

	if (sceneNodeParent) {
		transformNode = mScene->CreateNode(transform, sceneNodeParent);
	} else {
		transformNode = mScene->CreateNode(transform, NULL);
	}

	CameraEntity *camera = mScene->CreateCamera();
	camera->LookAt(glm::vec3(target.x, target.y, -target.z));

	SceneNode *sceneNode = mScene->CreateNode(camera, transformNode);

	mScene->SetActiveCamera(sceneNode);

	Node *node = new Node(sceneNode);

	return node;
}

Node *Renderer::AddMesh(char *path, const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale, const bool isStatic, Node *parent) {
	SceneNode *sceneNodeParent = NULL;

	if (parent) {
		sceneNodeParent = parent->GetSceneNode();
	}

	TransformEntity *transform = mScene->CreateTransform(position, rotation, scale);
	SceneNode *transformNode = NULL;

	if (sceneNodeParent) {
		transformNode = mScene->CreateNode(transform, sceneNodeParent);
	} else {
		transformNode = mScene->CreateNode(transform, NULL);
	}

	MeshEntity *mesh = mScene->CreateMesh(path, isStatic);

	SceneNode *sceneNode = mScene->CreateNode(mesh, transformNode);

	if (isStatic) {
		mScene->UpdateBoundingBox(sceneNode);
	}

	Node *node = new Node(sceneNode);

	return node;
}

Node *Renderer::AddLight(const glm::vec3 &position, const glm::ivec4 &color, const float ambientIntensity, const float diffuseIntensity, const float attenuation, Node *parent) {
	SceneNode *sceneNodeParent = NULL;

	if (parent) {
		sceneNodeParent = parent->GetSceneNode();
	}

	TransformEntity *transform = mScene->CreateTransform(position);
	SceneNode *transformNode = NULL;

	if (sceneNodeParent) {
		transformNode = mScene->CreateNode(transform, sceneNodeParent);
	} else {
		transformNode = mScene->CreateNode(transform, NULL);
	}

	LightEntity *light = mScene->CreateLight(color, ambientIntensity, diffuseIntensity, attenuation);

	SceneNode *sceneNode = mScene->CreateNode(light, transformNode);

	mScene->RegisterLight(sceneNode);

	Node *node = new Node(sceneNode);

	return node;
}

Node *Renderer::AddPlane(const glm::vec3 &position, const int sizeX, const int sizeZ, Node *parent) {
	SceneNode *sceneNodeParent = NULL;

	if (parent) {
		sceneNodeParent = parent->GetSceneNode();
	}

	TransformEntity *transform = mScene->CreateTransform(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f * sizeX, 1.0f, 1.0f * sizeZ));
	SceneNode *transformNode = NULL;
	
	if (sceneNodeParent) {
		transformNode = mScene->CreateNode(transform, sceneNodeParent);
	} else {
		transformNode = mScene->CreateNode(transform, NULL);
	}

	MeshEntity *plane = mScene->CreateMesh("../Media/Meshes/Primitives/plane.km");

	SceneNode *sceneNode = mScene->CreateNode(plane, transformNode);

	Node *node = new Node(sceneNode);

	return node;
}

Node *Renderer::AddParticle(const glm::vec3 &position, const float size, const float lifetime, const glm::vec2 &velocityRange, const glm::vec3 &acceleration, const int numberOfParticles, const float width, const float dispersion, char *path, const glm::ivec4 &color, Node *parent) {
	SceneNode *sceneNodeParent = NULL;

	if (parent) {
		sceneNodeParent = parent->GetSceneNode();
	}

	TransformEntity *transform = mScene->CreateTransform(position);
	SceneNode *transformNode = NULL;

	if (sceneNodeParent) {
		transformNode = mScene->CreateNode(transform, sceneNodeParent);
	} else {
		transformNode = mScene->CreateNode(transform, NULL);
	}

	ParticleEntity *particle = mScene->CreateParticle(size, lifetime, velocityRange, acceleration, numberOfParticles, width, dispersion, path);
	
	particle->SetColor(color);

	SceneNode *sceneNode = mScene->CreateNode(particle, transformNode);

	Node *node = new Node(sceneNode);

	return node;
}

void Renderer::SetActiveCamera(Node *camera) {
	mScene->SetActiveCamera(camera->GetSceneNode());
}

void Renderer::SetLightsVisible(const bool visible) {
	mScene->SetLightsVisible(visible);
}

void Renderer::SetGlobalLightParameters(const glm::vec3 &position, const float intensity, const glm::ivec4 &color, const float offset) {
	mProgramManager->SetGlobalLightParameters(position, intensity, color, offset);
}