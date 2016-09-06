#include <Scene\Scene.h>

#include <iostream>
#include <Config.h>

Scene::Scene(ProgramManager *programManager, ResourceManager *resourceManager) {
	mRoot = new SceneNode(new TransformEntity(), NULL);
	mActiveCamera = NULL;
	mLights.resize(0);

	mProgramManager = programManager;
	mResourceManager = resourceManager;

	InitShadowFBO();
	InitPostProcessingFBO();
}

Scene::~Scene() {
	delete mRoot;
	glDeleteFramebuffers(1, &mShadowFBO);
	glDeleteFramebuffers(1, &mPostProcessingFBO);
}

void Scene::Clear() {
	delete mRoot;
	mRoot = new SceneNode(new TransformEntity(), NULL);
	mActiveCamera = NULL;
	mLights.resize(0);
}


SceneNode *Scene::CreateNode(Entity *entity, SceneNode *parent) {
	SceneNode *node;
	if (parent) {
		node = new SceneNode(entity, parent);
	} else {
		node = new SceneNode(entity, mRoot);
	}
	return node;
}

TransformEntity *Scene::CreateTransform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) {
	TransformEntity *transform = new TransformEntity(glm::vec3(position.x, position.y, -position.z), glm::vec3(rotation.x, rotation.y, rotation.z), glm::vec3(scale.x, scale.y, scale.z));
	return transform;
}

MeshEntity *Scene::CreateMesh(char *path, const bool isStatic) {
	MeshEntity *mesh = new MeshEntity(path, mProgramManager, mResourceManager, isStatic);
	return mesh;
}

CameraEntity *Scene::CreateCamera() {
	CameraEntity *camera = new CameraEntity();
	return camera;
}

LightEntity *Scene::CreateLight(const glm::ivec4 &color, const float ambientIntensity, const float diffuseIntensity, const float attenuation) {
	LightEntity *light = new LightEntity(color, ambientIntensity, diffuseIntensity, attenuation);
	return light;
}

ParticleEntity *Scene::CreateParticle(const float size, const float lifetime, const glm::vec2 &velocityRange, const glm::vec3 &acceleration, const int numberOfParticles, const float width, const float dispersion, char *path) {
	ParticleEntity *particle = new ParticleEntity(size, lifetime, glm::vec2(velocityRange.x, velocityRange.y), glm::vec3(acceleration.x, acceleration.y, -acceleration.z), numberOfParticles, width, dispersion, path, mProgramManager, mResourceManager);
	return particle;
}

void Scene::Draw() {
	if (!mLights.empty()) {
		InitLights();
	}

	if (mActiveCamera) {
		InitCamera();
	}

	if (mRoot) {
		DrawShadowMap();
	}

	PreparePostProcessing();

	// Draw scene tree.
	if (mRoot) {
		mMatrixStack.push(glm::mat4(1.0));	// Identity matrix first.
		mMovedStack.push(false);
		mRoot->Draw(mMatrixStack, mMovedStack, false);
		mMatrixStack.pop();
		mMovedStack.pop();
		mRoot->CheckDeleted(mListToDelete);
	}

	DrawPostProcessing();

	CheckDeletes();
}

void Scene::SetActiveCamera(SceneNode *camera) {
	mActiveCamera = camera;
}

void Scene::RegisterLight(SceneNode *light) {
	if (light) {
		if (std::find(mLights.begin(), mLights.end(), light) != mLights.end()) {
			// Already registered.
		} else {
			mLights.push_back(light);
		}
	}
}

void Scene::UnregisterLight(SceneNode *light) {
	for (unsigned int i = 0; i < mLights.size(); i++) {
		if (mLights[i] == light) {
			mLights.erase(mLights.begin() + i);
			break;
		}
	}
}

void Scene::InitLights() {
	// Clean previous iteration.
	mProgramManager->ClearActiveLights();

	SceneNode *node;
	std::vector<glm::mat4> matrices;
	glm::mat4 matrix = glm::mat4(1);
	for (unsigned int i = 0; i < mLights.size(); i++) {
		if (mLights[i]->IsVisible()) {
			node = mLights[i]->GetParent();
			while (node) {
				if (node->GetEntity()->GetType() == Entity::ENTITY_TRANSFORM) {
					matrices.push_back(static_cast<TransformEntity *>(node->GetEntity())->GetMatrix());
				}
				node = node->GetParent();
			}

			matrix = matrices[matrices.size() - 1];
			for (int j = matrices.size() - 2; j > -1; j--) {
				matrix *= matrices[j];
			}

			LightEntity *lightEntity = static_cast<LightEntity *>(mLights[i]->GetEntity());
			mProgramManager->AddActiveLight(matrix[3], lightEntity->GetColor(), lightEntity->GetAmbientIntensity(), lightEntity->GetDiffuseIntensity(), lightEntity->GetAttenuation());

			matrices.clear();
			matrix = glm::mat4(1);
		}
	}
}

void Scene::InitCamera() {

	CameraEntity *cameraEntity = (CameraEntity *)mActiveCamera->GetEntity();

	SceneNode *node;
	std::vector<glm::mat4> matrices;
	glm::mat4 matrix = glm::mat4(1);

	node = mActiveCamera->GetParent();
	while (node) {
		if (node->GetEntity()->GetType() == Entity::ENTITY_TRANSFORM) {
			matrices.push_back(static_cast<TransformEntity *>(node->GetEntity())->GetMatrix());
		}
		node = node->GetParent();
	}

	matrix = matrices[matrices.size() - 1];
	for (int i = matrices.size() - 2; i > -1; i--) {
		matrix *= matrices[i];
	}

	cameraEntity->SetPosition(glm::vec3(matrix[3]));
	mProgramManager->ResetChangeFlags();
	mProgramManager->SetActiveCameraMatrix(cameraEntity->GetMatrix());
	mProgramManager->SetActiveCameraView(cameraEntity->GetView());
	mProgramManager->SetActiveCameraProjection(cameraEntity->GetProjection());
	mProgramManager->SetActiveCameraOrientation(cameraEntity->GetOrientation());
	mProgramManager->SetActiveCameraPosition(cameraEntity->GetPosition());
	mProgramManager->CalculateFrustumPlanes();
}

void Scene::CheckDeletes() {
	for (std::list<SceneNode *>::iterator iterator = mListToDelete.begin(); iterator != mListToDelete.end(); ++iterator) {
		SceneNode *node = *iterator;

		if (node->GetEntity()->GetType() == Entity::ENTITY_LIGHT) {
			UnregisterLight(node);
		} else if (node->GetEntity()->GetType() == Entity::ENTITY_CAMERA) {
			if (node == mActiveCamera) {
				mActiveCamera = NULL;
			}
		}
	}

	std::list<SceneNode *>::iterator iterator = mListToDelete.begin();
	SceneNode *node;
	while (mListToDelete.size()) {
		node = *iterator;
		if (node->GetChildCount() == 0) {
			delete node;
			iterator = mListToDelete.erase(iterator);
		} else {
			++iterator;
		}
		if (iterator == mListToDelete.end()) {
			iterator = mListToDelete.begin();
		}
	}
}

void Scene::SetLightsVisible(const bool visible) {
	std::vector<SceneNode*> lightsAux;

	if (visible) {
		lightsAux = mLightsPaused.top();
		mLightsPaused.pop();
		for (unsigned int i = 0; i < lightsAux.size(); i++) {
			lightsAux[i]->SetVisible(true);
		}
	} else {
		for (unsigned int i = 0; i < mLights.size(); i++) {
			if (mLights[i]->IsVisible()) {
				mLights[i]->SetVisible(false);
				lightsAux.push_back(mLights[i]);
			}
		}
		mLightsPaused.push(lightsAux);
	}
}

void Scene::UpdateBoundingBox(SceneNode *meshNode) {
	SceneNode *node;
	std::vector<glm::mat4> matrices;
	glm::mat4 matrix = glm::mat4(1.0f);

	node = meshNode->GetParent();
	while (node) {
		if (node->GetEntity()->GetType() == Entity::ENTITY_TRANSFORM) {
			matrices.push_back(static_cast<TransformEntity *>(node->GetEntity())->GetMatrix());
		}
		node = node->GetParent();
	}

	for (int i = matrices.size() - 2; i > -1; i--) {
		matrices[i] *= matrices[i + 1];
	}

	glm::vec4 maxTotal, minTotal, maxAux, minAux;

	MeshEntity *meshAux = static_cast<MeshEntity *>(meshNode->GetEntity());
	maxTotal = matrices[0] * meshAux->GetOriginalMax();
	minTotal = matrices[0] * meshAux->GetOriginalMin();

	meshAux->SetMax(maxTotal);
	meshAux->SetMin(minTotal);

	node = meshNode->GetParent();

	int i = 0;
	while (node) {
		if (node->GetEntity()->GetType() == Entity::ENTITY_MESH) {
			meshAux = static_cast<MeshEntity*>(node->GetEntity());
			maxAux = meshAux->GetMax();
			minAux = meshAux->GetMin();

			if (maxAux.x > maxTotal.x)	maxTotal.x = maxAux.x;
			if (maxAux.y > maxTotal.y)	maxTotal.y = maxAux.y;
			if (maxAux.z > maxTotal.z)	maxTotal.z = maxAux.z;
			if (minAux.x < minTotal.x)	minTotal.x = minAux.x;
			if (minAux.y < minTotal.y)	minTotal.y = minAux.y;
			if (minAux.z < minTotal.z)	minTotal.z = minAux.z;

			meshAux->SetMax(maxTotal);
			meshAux->SetMin(minTotal);

		} else if (node->GetEntity()->GetType() == Entity::ENTITY_TRANSFORM) {
			i++;
		}
		node = node->GetParent();
	}
}

void Scene::InitShadowFBO() {
	GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };

	mShadowMapWidth = 2048;
	mShadowMapHeight = 2048;

	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &mDepthTexture);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT16, mShadowMapWidth, mShadowMapHeight);
	// GL_LINEAR for PCF, GL_NEAREST for aliasing.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	glActiveTexture(GL_TEXTURE1);	// Framebuffer in GL_TEXTURE1
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);

	glGenFramebuffers(1, &mShadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

	GLenum drawBuffers[] = { GL_NONE };
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result = GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Shadow Map Framebuffer Complete\n";
	} else {
		std::cout << "Shadow Map Framebuffer Fail\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
}

void Scene::DrawShadowMap() {
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mDepthTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, mShadowMapWidth, mShadowMapHeight);
	glCullFace(GL_FRONT);

	mMatrixStack.push(glm::mat4(1.0));
	mMovedStack.push(false);
	mRoot->Draw(mMatrixStack, mMovedStack, true);
	mMatrixStack.pop();
	mMovedStack.pop();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, Config::width, Config::height);
	glCullFace(GL_BACK);
	glActiveTexture(GL_TEXTURE0);
}

void Scene::ShowShadowMap() {
	int size = mShadowMapWidth * mShadowMapHeight;
	float * buffer = new float[size];
	unsigned char * imgBuffer = new unsigned char[size * 4];

	glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, buffer);

	for (int i = 0; i < mShadowMapHeight; i++) {
		for (int j = 0; j < mShadowMapWidth; j++) {
			int imgIdx = 4 * ((i * mShadowMapWidth) + j);
			int bufIdx = ((mShadowMapHeight - i - 1) * mShadowMapWidth) + j;

			float minVal = 0.99f;
			float scale = (buffer[bufIdx] - minVal) / (1.0f - minVal);
			unsigned char val = (unsigned char)(scale * 255);
			imgBuffer[imgIdx] = val;
			imgBuffer[imgIdx + 1] = val;
			imgBuffer[imgIdx + 2] = val;
			imgBuffer[imgIdx + 3] = 0xff;
		}
	}

	mResourceManager->RemoveTexture("debugTex");
	mResourceManager->GetTexture("debugTex", imgBuffer, mShadowMapWidth, mShadowMapHeight);

	delete[] buffer;
	delete[] imgBuffer;
}

void Scene::InitPostProcessingFBO() {
	glGenFramebuffers(1, &mPostProcessingFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mPostProcessingFBO);

	glGenTextures(1, &mRenderTexture);
	glBindTexture(GL_TEXTURE_2D, mRenderTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		Config::width,
		Config::height,
		0, GL_RGBA, GL_UNSIGNED_BYTE,
		NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderTexture, 0);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Config::width, Config::height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glGenTextures(1, &mPostDepthTexture);
	glBindTexture(GL_TEXTURE_2D, mPostDepthTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, Config::width, Config::height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mPostDepthTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result = GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "PostProcessing Framebuffer Complete\n";
	} else {
		std::cout << "PostProcessing Framebuffer Fail\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mProgramPostProcessing = mProgramManager->Get("PostProcessing");
	mUniform_Width = mProgramPostProcessing->GetUniformLocation("Width");
	mUniform_Height = mProgramPostProcessing->GetUniformLocation("Height");
	mUniform_RenderTexture = mProgramPostProcessing->GetUniformLocation("Texture");
	mUniform_DepthTexture = mProgramPostProcessing->GetUniformLocation("DepthTexture");
}

void Scene::PreparePostProcessing() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mPostProcessingFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::DrawPostProcessing() {
	mProgramPostProcessing->Use();
	mProgramPostProcessing->SetUniform(mUniform_Width, Config::width);
	mProgramPostProcessing->SetUniform(mUniform_Height, Config::height);
	mProgramPostProcessing->SetUniform(mUniform_RenderTexture, 0);	// GL_TEXTURE0
	mProgramPostProcessing->SetUniform(mUniform_DepthTexture, 1);		// GL_TEXTURE1
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mRenderTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mPostDepthTexture);
	glDrawArrays(GL_POINTS, 0, 1);
	glActiveTexture(GL_TEXTURE0);
}