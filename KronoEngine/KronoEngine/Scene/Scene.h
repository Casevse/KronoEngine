#pragma once

#include <Resource\ResourceManager.h>
#include <Scene\SceneNode.h>
#include <Entity\TransformEntity.h>
#include <Entity\MeshEntity.h>
#include <Entity\CameraEntity.h>
#include <Entity\LightEntity.h>
#include <Entity\ParticleEntity.h>

class Scene {
public:

											Scene(ProgramManager *programManager, ResourceManager *resourceManager);
											~Scene();

	void									Clear();

	SceneNode								*CreateNode(Entity *entity, SceneNode *parent = NULL);
	
	TransformEntity							*CreateTransform(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &rotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f));
	MeshEntity								*CreateMesh(char *path, const bool isStatic = false);
	CameraEntity							*CreateCamera();
	LightEntity								*CreateLight(const glm::ivec4 &color, const float ambientIntensity, const float diffuseIntensity, const float attenuation);
	ParticleEntity							*CreateParticle(const float size, const float lifetime, const glm::vec2 &velocityRange, const glm::vec3 &acceleration, const int numberOfParticles, const float width, const float dispersion, char *path);

	void									Draw();

	void									SetActiveCamera(SceneNode *camera);
	void									RegisterLight(SceneNode *light);
	void									UnregisterLight(SceneNode *light);

	void									InitLights();
	void									InitCamera();
	void									CheckDeletes();

	void									SetLightsVisible(const bool visible);
	void									UpdateBoundingBox(SceneNode *meshNode);

	void									ShowTree();

private:

	SceneNode								*mRoot;
	SceneNode								*mActiveCamera;
	std::vector<SceneNode*>					mLights;

	std::stack<glm::mat4>					mMatrixStack;
	std::stack<bool>						mMovedStack;

	ProgramManager							*mProgramManager;
	ResourceManager							*mResourceManager;

	std::list<SceneNode*>					mListToDelete;

	std::stack<std::vector<SceneNode*>>		mLightsPaused;

	// Shadow Map.
	void									InitShadowFBO();
	void									DrawShadowMap();
	void									ShowShadowMap();
	GLuint									mShadowFBO;
	GLuint									mDepthTexture;
	int										mShadowMapWidth;
	int										mShadowMapHeight;

	// Post Processing.
	void									InitPostProcessingFBO();
	void									PreparePostProcessing();
	void									DrawPostProcessing();
	GLuint									mPostProcessingFBO;
	GLuint									mRenderTexture;
	Program									*mProgramPostProcessing;
	GLint									mUniform_Width;
	GLint									mUniform_Height;
	GLuint									mPostDepthTexture;
	GLint									mUniform_RenderTexture;
	GLint									mUniform_DepthTexture;

};