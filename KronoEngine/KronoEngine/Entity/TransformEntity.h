#pragma once

#include <Entity\Entity.h>

class TransformEntity : public Entity {
public:

	TransformEntity(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &rotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f));
	~TransformEntity();

	void		SetMatrix(const glm::mat4 &matrix);
	glm::mat4	GetMatrix();

	void		SetPosition(const glm::vec3 &position);
	glm::vec3	GetPosition();

	void		SetRotation(const glm::vec3 &rotation);
	glm::vec3	GetRotation();

	void		SetScale(const glm::vec3 &scale);
	glm::vec3	GetScale();

	bool		BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap);
	void		EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack);
	
private:

	glm::mat4	mMatrix;
	glm::vec3	mPosition;
	glm::vec3	mRotation;
	glm::vec3	mScale;

	// Auxiliar matrix to save calculations.
	glm::mat4	mPositionMatrix;
	glm::mat4	mRotationMatrix;
	glm::mat4	mScaleMatrix;

	bool		mMoved;
	glm::mat4	mLastMatrix;

	void		UpdateMatrix();

};