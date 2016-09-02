#pragma once

#include <Entity\Entity.h>

#include <Resource\ResourceManager.h>

class ParticleEntity : public Entity {
public:

						ParticleEntity(const float size, const float lifetime, const glm::vec2 &velocityRange, const glm::vec3 &acceleration,
							const int numberOfParticles, const float width, const float dispersion, char *path, ProgramManager *programManager, ResourceManager *resourceManager);
						~ParticleEntity();

	bool				BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap);
	void				EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack);

	void				SetTexture(char *path);
	void				ClearTexture();

	void				SetColor(const glm::ivec4 &color);
	glm::ivec4			GetColor();
	void				SetColorAlpha(const int Alpha);

private:

	void				InitBuffers();

	Program				*mProgram;
	ResourceManager		*mResourceManager;
	TextureResource		*mTexture;
	glm::vec4			mColor;
	float				mSize;
	float				mLifetime;
	glm::vec2			mVelocityRange;
	glm::vec3			mAcceleration;
	int					mNumberOfParticles;
	float				mWidth;
	float				mDispersion;

	GLuint				mInitialVelocity;
	GLuint				mStartTime;
	GLuint				mInitialPosition;
	GLuint				mParticles;

	// Uniform locations.
	GLint				mUniform_MVP;
	GLint				mUniform_Time;
	GLint				mUniform_ParticleLifetime;
	GLint				mUniform_Acceleration;
	GLint				mUniform_Size;
	GLint				mUniform_Color;
	GLint				mUniform_ParticleTexture;

};