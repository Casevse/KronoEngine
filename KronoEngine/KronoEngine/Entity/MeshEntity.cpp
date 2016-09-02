#include <Entity\MeshEntity.h>

MeshEntity::MeshEntity(char *path, ProgramManager *programManager, ResourceManager *resourceManager, const bool isStatic) {
	SetType(ENTITY_MESH);

	mProgramManager = programManager;
	mResourceManager = resourceManager;

	mMesh = resourceManager->GetMesh(path);

	mMax = glm::vec4(mMesh->GetMaxPoint(), 1.0f);
	mMin = glm::vec4(mMesh->GetMinPoint(), 1.0f);
	mCenter = mMesh->GetCenterPoint();
	mRadius = mMesh->GetRadius();

	mTexture = NULL;

	mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	mStatic = isStatic;
}

MeshEntity::~MeshEntity() {

}

bool MeshEntity::BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap) {
	if (shadowMap) {
		mMesh->DrawShadowMap(matrixStack.top());
		if (mStatic) {
			return true;
		}
	} else if (CheckFrustumCulling(matrixStack.top(), movedStack.top(), false)) {
		if (movedStack.top() || mProgramManager->GetActiveCameraViewChanged()) {
			mMV = mProgramManager->GetActiveCameraView() * matrixStack.top();
			mNormalMatrix = glm::mat3(glm::vec3(mMV[0]), glm::vec3(mMV[1]), glm::vec3(mMV[2]));
			mMVP = mProgramManager->GetActiveCameraProjection() * mMV;
		}

		mMesh->Draw(matrixStack.top(), mMV, mNormalMatrix, mMVP, mTexture, mColor);

		if (mStatic) {
			return true;
		}
	}

	if (mStatic) {
		return false;
	} else {
		return true;
	}
}

void MeshEntity::EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack) {

}

void MeshEntity::SetTexture(char *path) {
	mTexture = mResourceManager->GetTexture(path, true);
	mTexture->SetRepeat(true);
}

void MeshEntity::ClearTexture() {
	mTexture = NULL;
}

void MeshEntity::SetColor(const glm::ivec4 &color) {
	mColor.r = color.r / 255.0f;
	mColor.g = color.g / 255.0f;
	mColor.b = color.b / 255.0f;
	mColor.a = color.a / 255.0f;
}

glm::ivec4 MeshEntity::GetColor() {
	return glm::ivec4((int)(mColor.r * 255.0f), (int)(mColor.g * 255.0f), (int)(mColor.b * 255.0f), (int)(mColor.a * 255.0f));
}

void MeshEntity::SetColorAlpha(const int alpha) {
	mColor.a = (float)alpha / 255.0f;
}

void MeshEntity::SetMax(const glm::vec4 &max) {
	mMax = max;
}

glm::vec4 MeshEntity::GetMax() {
	return mMax;
}

void MeshEntity::SetMin(const glm::vec4 &min) {
	mMin = min;
}

glm::vec4 MeshEntity::GetMin() {
	return mMin;
}

glm::vec4 MeshEntity::GetOriginalMax() {
	return glm::vec4(mMesh->GetMaxPoint(), 1.0f);
}

glm::vec4 MeshEntity::GetOriginalMin() {
	return glm::vec4(mMesh->GetMinPoint(), 1.0f);
}