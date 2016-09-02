#include <Entity\ParticleEntity.h>

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\random.hpp>

ParticleEntity::ParticleEntity(const float size, const float lifetime, const glm::vec2 &velocityRange, const glm::vec3 &acceleration,
	const int numberOfParticles, const float width, const float dispersion, char *path, ProgramManager *programManager, ResourceManager *resourceManager)
{
	SetType(ENTITY_PARTICLE);

	mProgramManager = programManager;
	mResourceManager = resourceManager;

	mTexture = mResourceManager->GetTexture(path, true);

	mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	mSize = size;
	mLifetime = lifetime;
	mVelocityRange = velocityRange;
	mAcceleration = acceleration;
	mNumberOfParticles = numberOfParticles;
	mWidth = width;
	mDispersion = dispersion;

	mProgram = mProgramManager->Get("Particle");

	InitBuffers();

	mStatic = false;

	mUniform_MVP = mProgram->GetUniformLocation("MVP");
	mUniform_Time = mProgram->GetUniformLocation("Time");
	mUniform_ParticleLifetime = mProgram->GetUniformLocation("ParticleLifetime");
	mUniform_Acceleration = mProgram->GetUniformLocation("Acceleration");
	mUniform_Size = mProgram->GetUniformLocation("Size");
	mUniform_Color = mProgram->GetUniformLocation("Color");
	mUniform_ParticleTexture = mProgram->GetUniformLocation("ParticleTexture");
}

ParticleEntity::~ParticleEntity() {
	glDeleteVertexArrays(1, &mParticles);
	glDeleteBuffers(1, &mInitialPosition);
	glDeleteBuffers(1, &mStartTime);
	glDeleteBuffers(1, &mInitialPosition);
}

bool ParticleEntity::BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap) {
	if (!shadowMap) {
		if (CheckFrustumCulling(matrixStack.top(), movedStack.top(), false)) {
			mProgram->Use();

			mProgram->SetUniform(mUniform_MVP, mProgramManager->GetActiveCameraProjection() * mProgramManager->GetActiveCameraView() * matrixStack.top());
			// TODO method to get the time.
			//mProgram->SetUniform(mUniform_Time, (float)Graphics::Singleton()->GetTime() / 1000.0f);

			mProgram->SetUniform(mUniform_ParticleLifetime, mLifetime);
			mProgram->SetUniform(mUniform_Acceleration, mAcceleration);
			mProgram->SetUniform(mUniform_Size, mSize);
			mProgram->SetUniform(mUniform_Color, mColor);

			if (mTexture) {
				mProgram->SetUniform(mUniform_ParticleTexture, 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mTexture->GetTextureObject());
			}

			glBindVertexArray(mParticles);

			// Interesting effect.
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			glDrawArrays(GL_POINTS, 0, mNumberOfParticles);

			if (mTexture) {
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}
	return true;
}

void ParticleEntity::EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack) {

}

void ParticleEntity::SetTexture(char *path) {
	mTexture = mResourceManager->GetTexture(path, true);
}

void ParticleEntity::ClearTexture() {
	mTexture = NULL;
}

void ParticleEntity::SetColor(const glm::ivec4 &color) {
	mColor.r = color.r / 255.0f;
	mColor.g = color.g / 255.0f;
	mColor.b = color.b / 255.0f;
	mColor.a = color.a / 255.0f;
}

glm::ivec4 ParticleEntity::GetColor() {
	return glm::ivec4((int)(mColor.r * 255.0f), (int)(mColor.g * 255.0f), (int)(mColor.b * 255.0f), (int)(mColor.a * 255.0f));
}

void ParticleEntity::SetColorAlpha(const int alpha) {
	mColor.a = (float)alpha / 255.0f;
}

void ParticleEntity::InitBuffers() {
	mMax = glm::vec4(0.0f, 0.01f, 0.0f, 0.0f);
	mMin = glm::vec4(0.0f, -0.01f, 0.0f, 0.0f);

	glGenBuffers(1, &mInitialVelocity);
	glGenBuffers(1, &mStartTime);
	glGenBuffers(1, &mInitialPosition);

	int size = mNumberOfParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, mInitialVelocity);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, mStartTime);
	glBufferData(GL_ARRAY_BUFFER, mNumberOfParticles * sizeof(float), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, mInitialPosition);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

	glm::vec3 v(0.0f);
	float velocity, theta, phi, yHeight;
	GLfloat *data = new GLfloat[mNumberOfParticles * 3];
	for (int i = 0; i < mNumberOfParticles; i++) {
		theta = glm::mix(0.0f, glm::radians<float>(mDispersion / 2.0f), glm::linearRand<float>(0.0f, 1.0f));
		phi = glm::mix(0.0f, glm::pi<float>() * 2.0f, glm::linearRand<float>(0.0f, 1.0f));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(mVelocityRange.x, mVelocityRange.y, glm::linearRand<float>(0.0f, 1.0f));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;

		yHeight = v.y * mLifetime;

		if (yHeight > mMax.y) {
			mMax.y = yHeight;
		} else if (yHeight < mMin.y) {
			mMin.y = yHeight;
		}
	}

	mMax.y += mSize * 0.0001f;
	mMin.y -= mSize * 0.0001f;

	if (mAcceleration.y > 0.0f) {
		mMax.y += mAcceleration.y * mLifetime;
	} else if (mAcceleration.y < 0.0f) {
		mMin.y += mAcceleration.y * mLifetime;
	}

	glBindBuffer(GL_ARRAY_BUFFER, mInitialVelocity);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mSize, data);
	delete[] data;

	data = new GLfloat[mNumberOfParticles];
	float time = 0.0f;
	float rate = mLifetime / (float)mNumberOfParticles;
	for (int i = 0; i < mNumberOfParticles; i++) {
		data[i] = time;
		time += rate;
	}

	glBindBuffer(GL_ARRAY_BUFFER, mStartTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mNumberOfParticles * sizeof(float), data);
	delete[] data;

	data = new GLfloat[mNumberOfParticles * 3];
	float xWidth, zWidth;
	for (int i = 0; i < mNumberOfParticles; i++) {
		theta = glm::mix(0.0f, glm::pi<float>() * mWidth, glm::linearRand<float>(0.0f, 1.0f));
		phi = glm::mix(0.0f, glm::pi<float>() * 2.0f, glm::linearRand<float>(0.0f, 1.0f));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		xWidth = v.x * mWidth;
		zWidth = v.z * mWidth;

		data[3 * i] = xWidth;
		data[3 * i + 1] = 0.0f;
		data[3 * i + 2] = zWidth;

		if (xWidth > mMax.x) {
			mMax.x = xWidth;
		} else if (xWidth < mMin.x) {
			mMin.x = xWidth;
		}

		if (zWidth > mMax.z) {
			mMax.z = zWidth;
		} else if (zWidth < mMin.z) {
			mMin.z = zWidth;
		}
	}

	mMax.x += mSize * 0.0001f;
	mMin.x -= mSize * 0.0001f;
	mMax.z += mSize * 0.0001f;
	mMin.z -= mSize * 0.0001f;

	if (mAcceleration.x > 0.0f) {
		mMax.x += mAcceleration.x * mLifetime;
	} else if (mAcceleration.x < 0.0f) {
		mMin.x += mAcceleration.x * mLifetime;
	}
	if (mAcceleration.z > 0.0f) {
		mMax.z += mAcceleration.z * mLifetime;
	} else if (mAcceleration.z < 0.0f) {
		mMin.z += mAcceleration.z * mLifetime;
	}

	mCenter = glm::vec4((mMax.x + mMin.x) / 2.0f, (mMax.y + mMin.y) / 2.0f, (mMax.z + mMin.z) / 2.0f, 1.0f);
	mRadius = glm::distance(mMax, mMin) / 2.0f;

	glBindBuffer(GL_ARRAY_BUFFER, mInitialPosition);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mSize, data);
	delete[] data;

	glGenVertexArrays(1, &mParticles);
	glBindVertexArray(mParticles);

	glBindBuffer(GL_ARRAY_BUFFER, mInitialVelocity);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mStartTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, mInitialPosition);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}