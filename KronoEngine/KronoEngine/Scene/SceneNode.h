#pragma once

#include <glm\glm.hpp>
#include <Entity\Entity.h>
#include <list>
#include <vector>

class SceneNode {
public:

								SceneNode(Entity *entity = NULL, SceneNode *parent = NULL);
								~SceneNode();

	int							AddChild(SceneNode *child);
	int							RemoveChild(SceneNode *child);

	void						SetDeleted(const bool deleted);
	bool						GetDeleted();

	bool						SetEntity(Entity *entity);
	Entity						*GetEntity();

	void						SetParent(SceneNode *parent);
	SceneNode					*GetParent();

	void						Draw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap);
	void						CheckDeleted(std::list<SceneNode*> &listToDelete);

	void						SetVisible(const bool visible);
	bool						IsVisible();

	int							GetChildCount();

	std::vector<SceneNode*>		&GetChilds();

private:

	bool						mDeleted;
	Entity						*mEntity;
	std::vector<SceneNode*>		mChilds;
	SceneNode					*mParent;
	bool						mVisible;

};