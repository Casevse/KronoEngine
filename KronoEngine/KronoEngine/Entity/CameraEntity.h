#pragma once

#include <Entity\Entity.h>

class CameraEntity : public Entity {
public:

						CameraEntity();
						~CameraEntity();

	bool				BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap);
	void				EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack);

	void				SetPosition(const glm::vec3 &position);
	const glm::vec3		&GetPosition() const;

	void				SetOffsetPosition(const glm::vec3 &offset);

	void				SetFieldOfView(float fieldOfView);
	float				GetFieldOfView() const;

	void				SetNearAndFarPlanes(float nearPlane, float farPlane);

	float				GetNearPlane() const;
	float				GetFarPlane() const;

	glm::mat4			GetOrientation() const;

	void				SetOffsetOrientation(float upAngle, float rightAngle);

	void				LookAt(const glm::vec3 &position);
	void				SetTarget(const glm::vec3 &target);
	glm::vec3			GetTarget() const;

	void				SetViewportAspectRatio(float viewportAspectRatio);
	float				GetViewportAspectRatio() const;

	glm::vec3			GetForward() const;
	glm::vec3			GetRight() const;
	glm::vec3			GetUp() const;
	glm::mat4			GetProjection() const;
	glm::mat4			GetView() const;
	glm::mat4			GetMatrix() const;

private:

	glm::vec3			mPosition;
	float				mHorizontalAngle;
	float				mVerticalAngle;
	float				mFieldOfView;
	float				mNearPlane;
	float				mFarPlane;
	float				mViewportAspectRatio;
	glm::vec3			mTarget;

	void				NormalizeAngles();
	const float			kMaxVerticalAngle = 89.0f;

};