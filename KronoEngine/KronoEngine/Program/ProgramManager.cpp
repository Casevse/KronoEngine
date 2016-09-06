#include <Program\ProgramManager.h>

#include <glm\gtc\matrix_access.hpp>
#include <glm\gtx\transform.hpp>

ProgramManager::ProgramManager() {
	SetGlobalLightParameters(glm::vec3(1000.0f, 1000.0f, 1000.0f), 0.6f, glm::ivec4(255, 255, 255, 255), 100.0f);
	
	mBias = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
		glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
}

ProgramManager::~ProgramManager() {
	Clear();
}

void ProgramManager::Add(const std::string &name, Program *program) {
	std::map<std::string, Program*>::iterator iterator;
	iterator = mPrograms.find(name);

	if (iterator == mPrograms.end()) {
		mPrograms[name] = program;
	}
}

void ProgramManager::Remove(const std::string &name) {
	std::map<std::string, Program*>::iterator iterator;
	iterator = mPrograms.find(name);

	if (iterator != mPrograms.end()) {
		Program *program = (*iterator).second;
		delete program;
		mPrograms.erase(iterator);
	}
}

void ProgramManager::Clear() {
	std::map<std::string, Program*>::iterator iterator, end = mPrograms.end();
	for (iterator = mPrograms.begin(); iterator != end; ++iterator) {
		Program *program = (*iterator).second;
		delete program;
	}
}

Program *ProgramManager::Get(const std::string &name) {
	std::map<std::string, Program*>::iterator iterator;
	iterator = mPrograms.find(name);

	if (iterator != mPrograms.end()) {
		Program *program = (*iterator).second;
		return program;
	} else {
		return NULL;
	}
}

void ProgramManager::SetActiveCameraMatrix(const glm::mat4 &matrix) {
	mActiveCameraMatrix = matrix;
}

glm::mat4 ProgramManager::GetActiveCameraMatrix() {
	return mActiveCameraMatrix;
}

void ProgramManager::SetActiveCameraView(const glm::mat4 &view) {
	if (view != mActiveCameraView) {
		mActiveCameraView = view;
		mActiveCameraViewChanged = true;
	}
}

glm::mat4 ProgramManager::GetActiveCameraView() {
	return mActiveCameraView;
}

const bool ProgramManager::GetActiveCameraViewChanged() {
	return mActiveCameraViewChanged;
}

void ProgramManager::SetActiveCameraProjection(const glm::mat4 &projection) {
	if (projection != mActiveCameraProjection) {
		mActiveCameraProjection = projection;
		mActiveCameraProjectionChanged = true;
	}
}

glm::mat4 ProgramManager::GetActiveCameraProjection() {
	return mActiveCameraProjection;
}

const bool ProgramManager::GetActiveCameraProjectionChanged() {
	return mActiveCameraProjectionChanged;
}

void ProgramManager::SetActiveCameraOrientation(const glm::mat4 &orientation) {
	mActiveCameraOrientation = orientation;
}

glm::mat4 ProgramManager::GetActiveCameraOrientation() {
	return mActiveCameraOrientation;
}

void ProgramManager::SetActiveCameraPosition(const glm::vec3 &position) {
	mActiveCameraPosition = position;
}

glm::vec3 ProgramManager::GetActiveCameraPosition() {
	return mActiveCameraPosition;
}

void ProgramManager::ResetChangeFlags() {
	mActiveCameraViewChanged = false;
	mActiveCameraProjectionChanged = false;
}

void ProgramManager::CalculateFrustumPlanes() {
	glm::mat4 VP = GetActiveCameraMatrix();
	glm::vec4 Row3 = glm::row(VP, 3);

	mFrustumPlanes[0] = glm::row(VP, 0) + Row3;	// Left
	mFrustumPlanes[1] = -glm::row(VP, 0) + Row3;	// Right
	mFrustumPlanes[2] = glm::row(VP, 1) + Row3;
	mFrustumPlanes[3] = -glm::row(VP, 1) + Row3;
	mFrustumPlanes[4] = glm::row(VP, 2) + Row3;
	mFrustumPlanes[5] = -glm::row(VP, 2) + Row3;
}

glm::vec4 *ProgramManager::GetFrustumPlanes() {
	return mFrustumPlanes;
}

void ProgramManager::InitPrograms() {
	Program *program;

	// ToonMesh (with/without texture)
	program = new Program();
	program->CompileShader("../Media/Shaders/ToonMesh.vs", GLSLShader::VERTEX);
	program->CompileShader("../Media/Shaders/ToonMesh.gs", GLSLShader::GEOMETRY);
	program->CompileShader("../Media/Shaders/ToonMesh.fs", GLSLShader::FRAGMENT);
	program->Link();
	Add("ToonMesh", program);

	// ToonSilhouette
	program = new Program();
	program->CompileShader("../Media/Shaders/ToonSilhouette.vs", GLSLShader::VERTEX);
	program->CompileShader("../Media/Shaders/ToonSilhouette.gs", GLSLShader::GEOMETRY);
	program->CompileShader("../Media/Shaders/ToonSilhouette.fs", GLSLShader::FRAGMENT);
	program->Link();
	Add("ToonSilhouette", program);

	// Particles
	program = new Program();
	program->CompileShader("../Media/Shaders/Particle.vs", GLSLShader::VERTEX);
	program->CompileShader("../Media/Shaders/Particle.fs", GLSLShader::FRAGMENT);
	program->Link();
	Add("Particle", program);

	// Shadow Map
	program = new Program();
	program->CompileShader("../Media/Shaders/ShadowMap.vs", GLSLShader::VERTEX);
	program->CompileShader("../Media/Shaders/ShadowMap.gs", GLSLShader::GEOMETRY);
	program->CompileShader("../Media/Shaders/ShadowMap.fs", GLSLShader::FRAGMENT);
	program->Link();
	Add("ShadowMap", program);

	// PostProcessing
	program = new Program();
	program->CompileShader("../Media/Shaders/PostProcessing.vs", GLSLShader::VERTEX);
	program->CompileShader("../Media/Shaders/PostProcessing.gs", GLSLShader::GEOMETRY);
	program->CompileShader("../Media/Shaders/PostProcessing.fs", GLSLShader::FRAGMENT);
	program->Link();
	Add("PostProcessing", program);
}

void ProgramManager::SetGlobalLightParameters(const glm::vec3 &position, const float intensity, const glm::ivec4 &color, const float offset) {
	mGlobalLight.mPosition = glm::vec4(position.x, position.y, -position.z, 0.0f);
	mGlobalLight.mIntensity = intensity;
	mGlobalLight.mColor = glm::vec3(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

	mGlobalLightView = glm::lookAt(glm::vec3(position.x, position.y, -position.z), glm::vec3(position.x + offset, position.y - offset, -position.z - offset), glm::vec3(0.0f, 1.0f, 0.0f));
	mGlobalLightProjection = glm::ortho<float>(-60, 60, -60, 60, 0.01f, 300.0f);

	mGlobalLightVPB = mBias * mGlobalLightProjection * mGlobalLightView;
}

GlobalLight &ProgramManager::GetGlobalLightParameters() {
	return mGlobalLight;
}

void ProgramManager::AddActiveLight(const glm::vec4 &position, const glm::vec3 &color, const float ambientIntensity, const float diffuseIntensity, const float attenuation) {
	PointLight pointLight;
	pointLight.mPosition = position;
	pointLight.mColor = color;
	pointLight.mAmbientIntensity = ambientIntensity;
	pointLight.mDiffuseIntensity = diffuseIntensity;
	pointLight.mAttenuation = attenuation;
	mActiveLights.push_back(pointLight);
}

std::vector<PointLight> &ProgramManager::GetActiveLights() {
	return mActiveLights;
}

void ProgramManager::ClearActiveLights() {
	mActiveLights.clear();
}

glm::mat4 &ProgramManager::GetGlobalLightView() {
	return mGlobalLightView;
}

glm::mat4 &ProgramManager::GetGlobalLightProjection() {
	return mGlobalLightProjection;
}

glm::mat4 &ProgramManager::GetGlobalLightVPB() {
	return mGlobalLightVPB;
}