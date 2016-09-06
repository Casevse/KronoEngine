#pragma once

#include <Scene\SceneNode.h>
#include <Entity\TransformEntity.h>

class Node {
public:

						Node(SceneNode *sceneNode);

	void				Remove();

	glm::vec3			GetPosition(const bool &absolute = false);
	glm::vec3			GetRotation();
	glm::vec3			GetScale();
	glm::vec3			GetTarget();
	glm::vec3			GetForward();
	glm::vec3			GetRight();
	glm::vec3			GetOffset(const float &yaw, const float &pitch);

	void				SetPosition(const glm::vec3 &position);
	void				SetRotation(const glm::vec3 &rotation);
	void				SetScale(const glm::vec3 &scale);
	void				SetTarget(const glm::vec3 &target);
	void				SetParent(Node *node);
	void				SetColor(const glm::ivec4 &color);

	void				SetTexture(char *path);
	void				SetTransparency(const int transparency);

	glm::vec3			GetBoundingBoxMax();
	glm::vec3			GetBoundingBoxMin();

	void				SetVisible(const bool visible);

	SceneNode			*GetSceneNode();

private:

	glm::mat4			GetAbsoluteMatrix();

	SceneNode			*mSceneNode;
	TransformEntity		*mTransformEntity;

};