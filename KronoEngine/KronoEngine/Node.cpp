#include <Node.h>

#include <Entity\CameraEntity.h>
#include <Entity\MeshEntity.h>
#include <Entity\ParticleEntity.h>
#include <glm\gtc\matrix_transform.hpp>

Node::Node(SceneNode *sceneNode) {
	mSceneNode = sceneNode;

	if (mSceneNode->GetEntity()->GetType() == Entity::ENTITY_TRANSFORM) {
		mTransformEntity = static_cast<TransformEntity*>(mSceneNode->GetEntity());
	} else {
		mTransformEntity = static_cast<TransformEntity*>(mSceneNode->GetParent()->GetEntity());
	}
}

void Node::Remove() {
	if (mSceneNode->GetEntity()->GetType() == Entity::ENTITY_TRANSFORM) {
		mSceneNode->SetDeleted(true);
	} else {
		mSceneNode->GetParent()->SetDeleted(true);
	}
}

glm::vec3 Node::GetPosition(const bool &absolute) {
	glm::vec3 position;

	if (absolute) {
		position = glm::vec3(GetAbsoluteMatrix()[3]);
	} else {
		position = mTransformEntity->GetPosition();
	}

	return glm::vec3(position.x, position.y, -position.z);
}

glm::vec3 Node::GetRotation() {
	glm::vec3 rotation = mTransformEntity->GetRotation();

	return glm::vec3(-rotation.x, -rotation.y, rotation.z);
}

glm::vec3 Node::GetScale() {
	glm::vec3 scale = mTransformEntity->GetScale();

	return glm::vec3(scale.x, scale.y, scale.z);
}

glm::vec3 Node::GetTarget() {
	if (mSceneNode->GetEntity()->GetType() == Entity::ENTITY_CAMERA) {
		glm::vec3 target = static_cast<CameraEntity*>(mSceneNode->GetEntity())->GetTarget();
		
		return glm::vec3(target.x, target.y, -target.z);
	} else {
		return glm::vec3();
	}
}

glm::vec3 Node::GetForward() {
	if (mSceneNode->GetEntity()->GetType() == Entity::ENTITY_CAMERA) {
		glm::vec3 forward = static_cast<CameraEntity*>(mSceneNode->GetEntity())->GetForward();

		return glm::vec3(forward.x, forward.y, -forward.z);
	} else {
		glm::mat4 matrix = GetAbsoluteMatrix();

		return glm::vec3(matrix[2].x, matrix[2].y, matrix[2].z);
	}
}

glm::vec3 Node::GetRight() {
	if (mSceneNode->GetEntity()->GetType() == Entity::ENTITY_CAMERA) {
		glm::vec3 right = static_cast<CameraEntity*>(mSceneNode->GetEntity())->GetRight();

		return glm::vec3(right.x, right.y, -right.z);
	} else {
		// TODO

		return glm::vec3();
	}
}

glm::vec3 Node::GetOffset(const float &yaw, const float &pitch) {
	glm::mat4 matrix(1.0f);

	matrix = glm::rotate(matrix, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::vec3 offset(0.0f, 0.0f, 1.0f);
	glm::vec3 aux;

	aux.x = offset.x * matrix[0].x + offset.y * matrix[1].x + offset.z * matrix[2].x + matrix[3].x;
	aux.y = offset.x * matrix[0].y + offset.y * matrix[1].y + offset.z * matrix[2].y + matrix[3].y;
	aux.z = offset.x * matrix[0].z + offset.y * matrix[1].z + offset.z * matrix[2].z + matrix[3].z;

	return aux;
}

void Node::SetPosition(const glm::vec3 &position) {
	mTransformEntity->SetPosition(glm::vec3(position.x, position.y, -position.z));
}

void Node::SetRotation(const glm::vec3 &rotation) {
	mTransformEntity->SetRotation(glm::vec3(-rotation.x, -rotation.y, rotation.z));
}

void Node::SetScale(const glm::vec3 &scale) {
	mTransformEntity->SetScale(glm::vec3(scale.x, scale.y, scale.z));
}

void Node::SetTarget(const glm::vec3 &target) {
	if (mSceneNode->GetEntity()->GetType() == Entity::ENTITY_CAMERA) {
		static_cast<CameraEntity*>(mSceneNode->GetEntity())->SetTarget(glm::vec3(target.x, target.y, -target.z));
	}
}

void Node::SetParent(Node *parent) {
	if (parent) {
		mSceneNode->SetParent(static_cast<Node*>(parent)->GetSceneNode());
	} else {
		mSceneNode->SetParent(NULL);
	}
}

void Node::SetColor(const glm::ivec4 &color) {
	Entity *entity = mSceneNode->GetEntity();

	if (entity->GetType() == Entity::ENTITY_MESH) {
		static_cast<MeshEntity*>(entity)->SetColor(color);
	} else if (entity->GetType() == Entity::ENTITY_PARTICLE) {
		static_cast<ParticleEntity*>(entity)->SetColor(color);
	}
}

void Node::SetTexture(char *path) {
	Entity *entity = mSceneNode->GetEntity();

	if (entity->GetType() == Entity::ENTITY_MESH) {
		if (path) {
			static_cast<MeshEntity*>(entity)->SetTexture(path);
		} else {
			static_cast<MeshEntity*>(entity)->ClearTexture();
		}
	} else if (entity->GetType() == Entity::ENTITY_PARTICLE) {
		if (path) {
			static_cast<ParticleEntity*>(entity)->SetTexture(path);
		} else {
			static_cast<ParticleEntity*>(entity)->ClearTexture();
		}
	}
}

void Node::SetTransparency(const int transparency) {
	Entity *entity = mSceneNode->GetEntity();

	if (entity->GetType() == Entity::ENTITY_MESH) {
		static_cast<MeshEntity*>(entity)->SetColorAlpha(transparency);
	} else if (entity->GetType() == Entity::ENTITY_PARTICLE) {
		static_cast<ParticleEntity*>(entity)->SetColorAlpha(transparency);
	}
}

glm::vec3 Node::GetBoundingBoxMax() {
	Entity *entity = mSceneNode->GetEntity();

	if (entity->GetType() == Entity::ENTITY_MESH) {
		glm::vec4 max = static_cast<MeshEntity*>(entity)->GetMax();
		return glm::vec3(max.x, max.y, -max.z);
	}
	return glm::vec3();
}

glm::vec3 Node::GetBoundingBoxMin() {
	Entity *entity = mSceneNode->GetEntity();

	if (entity->GetType() == Entity::ENTITY_MESH) {
		glm::vec4 min = static_cast<MeshEntity*>(entity)->GetMin();
		return glm::vec3(min.x, min.y, -min.z);
	}
	return glm::vec3();
}

void Node::SetVisible(const bool visible) {
	mSceneNode->SetVisible(visible);
}

SceneNode *Node::GetSceneNode() {
	return mSceneNode;
}

glm::mat4 Node::GetAbsoluteMatrix() {
	SceneNode *sceneNodeAux = mSceneNode;
	std::vector<glm::mat4> matrices;
	glm::mat4 matrix = glm::mat4(1);

	while (sceneNodeAux) {
		if (sceneNodeAux->GetEntity()->GetType() == Entity::ENTITY_TRANSFORM) {
			matrices.push_back(static_cast<TransformEntity*>(sceneNodeAux->GetEntity())->GetMatrix());
		}
		sceneNodeAux = sceneNodeAux->GetParent();
	}

	matrix = matrices[matrices.size() - 1];

	for (int i = matrices.size() - 2; i > -1; i--) {
		matrix *= matrices[i];
	}

	return matrix;
}