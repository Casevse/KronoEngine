#pragma once

#include <glm\glm.hpp>
#include <stack>

class Entity {
public:

				Entity();
	virtual		~Entity() {};

	virtual bool BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> movedStack, bool shawdowMap) = 0;
	virtual void EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack) = 0;

private:
	// TODO Frustum Culling
};