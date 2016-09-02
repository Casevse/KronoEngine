#pragma once

#include <Entity\Entity.h>

class LightEntity : public Entity {
public:

	LightEntity(const glm::ivec4 &color, const float ambientIntensity, const float diffuseIntensity, const float attenuation);
	~LightEntity();

	bool		BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap);
	void		EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack);

	void		SetColor(const glm::ivec4 &color);
	glm::vec3	GetColor();

	void		SetAmbientIntensity(const float intensity);
	float		GetAmbientIntensity();

	void		SetDiffuseIntensity(const float intensity);
	float		GetDiffuseIntensity();

	void		SetAttenuation(const float attenuation);
	float		GetAttenuation();

private:

	glm::vec3	mColor;
	float		mAmbientIntensity;
	float		mDiffuseIntensity;
	float		mAttenuation;

};