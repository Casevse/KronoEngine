#include <Scene\SceneNode.h>

SceneNode::SceneNode(Entity *entity, SceneNode *parent) {
	mEntity = entity;
	mParent = parent;
	mChilds.resize(0);
	mDeleted = false;
	mVisible = true;

	if (mParent) {
		mParent->AddChild(this);
	}
}

SceneNode::~SceneNode() {
	delete mEntity;

	if (mParent) {
		mParent->RemoveChild(this);
	}

	for (unsigned int i = 0; i < mChilds.size(); i++) {
		delete mChilds[i];
	}

	mChilds.clear();
}

int SceneNode::AddChild(SceneNode *child) {
	mChilds.push_back(child);
	return 1;
}

int SceneNode::RemoveChild(SceneNode *child) {
	for (unsigned int i = 0; i < mChilds.size(); i++) {
		if (mChilds[i] == child) {
			mChilds.erase(mChilds.begin() + i);
			break;
		}
	}
	return 1;
}

void SceneNode::SetDeleted(const bool deleted) {
	mDeleted = deleted;
	for (unsigned int i = 0; i < mChilds.size(); i++) {
		mChilds[i]->SetDeleted(true);
	}
}

bool SceneNode::GetDeleted() {
	return mDeleted;
}

bool SceneNode::SetEntity(Entity *entity) {
	mEntity = entity;
	return true;
}

Entity *SceneNode::GetEntity() {
	return mEntity;
}

void SceneNode::SetParent(SceneNode *parent) {
	// TODO
}

SceneNode *SceneNode::GetParent() {
	return mParent;
}

void SceneNode::Draw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap) {
	bool carryOn = false;

	if (mEntity) {
		carryOn = mEntity->BeginDraw(matrixStack, movedStack, shadowMap);
	}

	if (carryOn) {
		for (unsigned int i = 0; i < mChilds.size(); i++) {
			if (mChilds[i]->IsVisible()) {
				mChilds[i]->Draw(matrixStack, movedStack, shadowMap);
			}
		}
	}

	if (mEntity) {
		mEntity->EndDraw(matrixStack, movedStack);
	}
}

void SceneNode::CheckDeleted(std::list<SceneNode*> &listToDelete) {
	if (mDeleted) {
		listToDelete.push_back(this);
	}

	for (unsigned int i = 0; i < mChilds.size(); i++) {
		mChilds[i]->CheckDeleted(listToDelete);
	}
}

void SceneNode::SetVisible(const bool visible) {
	mVisible = visible;
}

bool SceneNode::IsVisible() {
	return mVisible;
}

int SceneNode::GetChildCount() {
	return mChilds.size();
}

std::vector<SceneNode*> &SceneNode::GetChilds() {
	return mChilds;
}