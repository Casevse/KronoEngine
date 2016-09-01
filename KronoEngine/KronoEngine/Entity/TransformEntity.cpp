#include <Entity\TransformEntity.h>

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

TransformEntity::TransformEntity(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) {
	SetType(ENTITY_TRANSFORM);

	mPosition = position;
	mRotation = rotation;
	mScale = scale;

	mPositionMatrix = glm::translate(mPositionMatrix, position);
	mRotationMatrix = glm::mat4(glm::quat(glm::vec3(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z))));
	mScaleMatrix = glm::scale(mScaleMatrix, scale);

	UpdateMatrix();
}

TransformEntity::~TransformEntity() {

}

void TransformEntity::SetMatrix(const glm::mat4 &matrix) {
	mMatrix = matrix;
}

glm::mat4 TransformEntity::GetMatrix() {
	return mMatrix;
}

void TransformEntity::SetPosition(const glm::vec3 &position) {
	mPosition = position;
	mPositionMatrix = glm::translate(mPositionMatrix, position);	// TODO Check if the matrix has to be de identity
	UpdateMatrix();
}

glm::vec3 TransformEntity::GetPosition() {
	return mPosition;
}

void TransformEntity::SetRotation(const glm::vec3 &rotation) {
	mRotation = rotation;
	mRotationMatrix = glm::mat4(glm::quat(glm::vec3(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z))));
	UpdateMatrix();
}

glm::vec3 TransformEntity::GetRotation() {
	return mRotation;
}

void TransformEntity::SetScale(const glm::vec3 &scale) {
	mScale = scale;
	mScaleMatrix = glm::scale(mScaleMatrix, scale);	// TODO Check if the matrix has to be de identity
	UpdateMatrix();
}

glm::vec3 TransformEntity::GetScale() {
	return mScale;
}

bool TransformEntity::BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap) {
	if (mMoved || movedStack.top()) {
		mLastMatrix = matrixStack.top() * mMatrix;

		if (mMoved && !shadowMap) {
			mMoved = false;
		}

		movedStack.push(true);
	} else {
		movedStack.push(false);
	}

	matrixStack.push(mLastMatrix);

	return true;
}

void TransformEntity::EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack) {
	matrixStack.pop();
	movedStack.pop();
}

void TransformEntity::UpdateMatrix() {
	mMatrix = mPositionMatrix * mRotationMatrix * mScaleMatrix;
	mMoved = true;
}