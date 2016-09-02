#include <Entity\LightEntity.h>

LightEntity::LightEntity(const glm::ivec4 &color, const float ambientIntensity, const float diffuseIntensity, const float attenuation) {
	SetType(ENTITY_LIGHT);

	mColor = glm::vec3((float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f);
	mAmbientIntensity = ambientIntensity;
	mDiffuseIntensity = diffuseIntensity;
	mAttenuation = attenuation;
}

LightEntity::~LightEntity() {

}

bool LightEntity::BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap) {
	return true;
}

void LightEntity::EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack) {

}

void LightEntity::SetColor(const glm::ivec4 &color) {
	mColor.r = (float)color.r / 255.0f;
	mColor.g = (float)color.g / 255.0f;
	mColor.b = (float)color.b / 255.0f;
}

glm::vec3 LightEntity::GetColor() {
	return mColor;
}

void LightEntity::SetAmbientIntensity(const float intensity) {
	mAmbientIntensity = intensity;
}

float LightEntity::GetAmbientIntensity() {
	return mAmbientIntensity;
}

void LightEntity::SetDiffuseIntensity(const float intensity) {
	mDiffuseIntensity = intensity;
}

float LightEntity::GetDiffuseIntensity() {
	return mDiffuseIntensity;
}

void LightEntity::SetAttenuation(const float attenuation) {
	mAttenuation = attenuation;
}

float LightEntity::GetAttenuation() {
	return mAttenuation;
}