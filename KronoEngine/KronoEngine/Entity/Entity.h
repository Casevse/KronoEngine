#pragma once

#include <glm\glm.hpp>
#include <stack>
#include <Program\ProgramManager.h>

class Entity {
public:

	enum EntityTypes {
		ENTITY_TRANSFORM,
		ENTITY_MESH,
		ENTITY_CAMERA,
		ENTITY_LIGHT,
		ENTITY_PARTICLE
	};

					Entity();
	virtual			~Entity() {};

	virtual bool	BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> movedStack, bool shawdowMap) = 0;
	virtual void	EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack) = 0;

	void			SetType(EntityTypes type);
	int				GetType();

protected:

	ProgramManager	*mProgramManager;

	// Frustum Culling
	enum ClipResult	{ INSIDE, OUTSIDE, INTERSECT };
	int				mPlaneCache;	// Save the last checking failed plane.
	glm::vec4		mMax;
	glm::vec4		mMin;
	glm::vec4		mVertices[8];
	bool			mStatic;

	glm::vec4		mCenter;
	float			mRadius;
	glm::vec4		mVertice;

	bool			CheckFrustumCulling(const glm::mat4 &model, const bool moved, const bool frameChanged);

private:
	
	EntityTypes		mType;

};