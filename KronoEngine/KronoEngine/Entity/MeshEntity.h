#pragma once

#include <Entity\Entity.h>
#include <Resource\ResourceManager.h>

class MeshEntity : public Entity {
public:

						MeshEntity(char *path, ProgramManager *programManager = NULL, ResourceManager *resourceManager = NULL, const bool isStatic = false);
						~MeshEntity();

	bool				BeginDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack, bool shadowMap);
	void				EndDraw(std::stack<glm::mat4> &matrixStack, std::stack<bool> &movedStack);

	void				SetTexture(char *path);
	void				ClearTexture();

	void				SetColor(const glm::ivec4 &color);
	glm::ivec4			GetColor();
	void				SetColorAlpha(const int alpha);

	void				SetMax(const glm::vec4 &max);
	glm::vec4			GetMax();

	void				SetMin(const glm::vec4 &min);
	glm::vec4			GetMin();

	glm::vec4			GetOriginalMax();
	glm::vec4			GetOriginalMin();

private:

	ResourceManager		*mResourceManager;
	MeshResource		*mMesh;
	TextureResource		*mTexture;
	glm::vec4			mColor;

	glm::mat4			mMV;
	glm::mat3			mNormalMatrix;
	glm::mat4			mMVP;

};