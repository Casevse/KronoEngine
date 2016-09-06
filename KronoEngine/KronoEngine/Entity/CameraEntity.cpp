#include <Entity\CameraEntity.h>

#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <Config.h>

CameraEntity::CameraEntity() :
	mPosition(0.0f, 0.0f, 1.0f),
	mHorizontalAngle(0.0f),
	mVerticalAngle(0.0f),
	mFieldOfView(glm::degrees(glm::pi<float>() / 2.5f)),
	mNearPlane(0.01f),
	mFarPlane(300.0f),
	mViewportAspectRatio(Config::width / Config::height)
{
	SetType(ENTITY_CAMERA);
}

CameraEntity::~CameraEntity() {

}

bool CameraEntity::BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap) {
	return true;
}

void CameraEntity::EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack) {

}

void CameraEntity::SetPosition(const glm::vec3 &position) {
	mPosition = position;
	LookAt(mTarget);
}

const glm::vec3 &CameraEntity::GetPosition() const {
	return mPosition;
}

void CameraEntity::SetOffsetPosition(const glm::vec3 &offset) {
	mPosition += offset;
}

void CameraEntity::SetFieldOfView(float fieldOfView) {
	if (fieldOfView > 0.0f && fieldOfView < 180.0f) {
		mFieldOfView = fieldOfView;
	}
}

float CameraEntity::GetFieldOfView() const {
	return mFieldOfView;
}

void CameraEntity::SetNearAndFarPlanes(float nearPlane, float farPlane) {
	if (nearPlane > 0.0f && farPlane > nearPlane) {
		mNearPlane = nearPlane;
		mFarPlane = farPlane;
	}
}

float CameraEntity::GetNearPlane() const {
	return mNearPlane;
}

float CameraEntity::GetFarPlane() const {
	return mFarPlane;
}

glm::mat4 CameraEntity::GetOrientation() const {
	glm::mat4 orientation;

	orientation = glm::rotate(orientation, glm::radians(mVerticalAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	orientation = glm::rotate(orientation, glm::radians(mHorizontalAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	return orientation;
}

void CameraEntity::SetOffsetOrientation(float upAngle, float rightAngle) {
	mHorizontalAngle += rightAngle;
	mVerticalAngle += upAngle;
	NormalizeAngles();
}

void CameraEntity::LookAt(const glm::vec3 &position) {
	if (position == mPosition) {
		return;
	}

	glm::vec3 direction = glm::normalize(position - mPosition);
	mVerticalAngle = glm::degrees(asinf(-direction.y));
	mHorizontalAngle = -glm::degrees(atan2f(-direction.x, -direction.z));
	
	NormalizeAngles();
}

void CameraEntity::SetTarget(const glm::vec3 &target) {
	mTarget = target;
}

glm::vec3 CameraEntity::GetTarget() const {
	return mTarget;
}

void CameraEntity::SetViewportAspectRatio(float viewportAspectRatio) {
	if (viewportAspectRatio > 0.0f) {
		mViewportAspectRatio = viewportAspectRatio;
	}
}

float CameraEntity::GetViewportAspectRatio() const {
	return mViewportAspectRatio;
}

glm::vec3 CameraEntity::GetForward() const {
	glm::vec4 forward = glm::inverse(GetOrientation()) * glm::vec4(0.0f, 0.0f, -1.0f, 1.f);

	return glm::vec3(forward);
}

glm::vec3 CameraEntity::GetRight() const {
	glm::vec4 right = glm::inverse(GetOrientation()) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	return glm::vec3(right);
}

glm::vec3 CameraEntity::GetUp() const {
	glm::vec4 up = glm::inverse(GetOrientation()) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	return glm::vec3(up);
}

glm::mat4 CameraEntity::GetProjection() const {
	return glm::perspective(glm::radians(mFieldOfView), mViewportAspectRatio, mNearPlane, mFarPlane);
}

glm::mat4 CameraEntity::GetView() const {
	return GetOrientation() * glm::translate(glm::mat4(), -mPosition);
}

glm::mat4 CameraEntity::GetMatrix() const {
	return GetProjection() * GetView();
}

void CameraEntity::NormalizeAngles() {
	mHorizontalAngle = fmodf(mHorizontalAngle, 360.0f);

	if (mHorizontalAngle < 0.0f) {
		mHorizontalAngle += 360.0f;
	}

	if (mVerticalAngle > kMaxVerticalAngle) {
		mVerticalAngle = kMaxVerticalAngle;
	} else if (mVerticalAngle < -kMaxVerticalAngle) {
		mVerticalAngle = -kMaxVerticalAngle;
	}
}