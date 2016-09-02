#pragma once

#include <Resource\Resource.h>
#include <Resource\TextureResource.h>
#include <Program\ProgramManager.h>

class MeshResource : public Resource {
public:

						MeshResource(char *path, ProgramManager *programManager);
						~MeshResource();

	// TODO Add texture resource.
	void				Draw(const glm::mat4 &model, const glm::mat4 &modelView, const glm::mat3 &normalMatrix, const glm::mat4 &modelViewProjection, TextureResource *texture = NULL, const glm::vec4 &color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void				DrawShadowMap(const glm::mat4 &model);

	// Bounding Box
	const glm::vec3		&GetMaxPoint() const;
	const glm::vec3		&GetMinPoint() const;

	// Bounding Sphere
	const glm::vec4		&GetCenterPoint() const;
	const float			GetRadius() const;

private:

	ProgramManager		*mProgramManager;

	void				LoadKMBin(char *path);	// KM = KronoMesh, own format.

	unsigned int		mNumFaces;
	unsigned int		mVaoHandle;
	unsigned int		mVaoHandleSilhouette;

	unsigned int		mHandle[4];
	unsigned int		mHandleSilhouette[2];

	TextureResource		*mTexture;

	Program				*mProgramMesh;
	Program				*mProgramSilhouette;
	Program				*mProgramShadowMap;

	glm::vec3			mMaxPoint;
	glm::vec3			mMinPoint;
	glm::vec4			mCenterPoint;
	float				mRadius;

	GLint				mUniformMesh_VertexColor;
	GLint				mUniformMesh_ModelViewMatrix;
	GLint				mUniformMesh_NormalMatrix;
	GLint				mUniformMesh_MVP;
	GLint				mUniformMesh_HasShadowMap;
	GLint				mUniformMesh_ShadowMatrix;
	GLint				mUniformMesh_ShadowMap;
	GLint				mUniformMesh_SunPosition;
	GLint				mUniformMesh_SunIntensity;
	GLint				mUniformMesh_SunColor;
	GLint				mUniformMesh_NumPointLights;
	GLint				mUniformMesh_PointLights_Position[4];
	GLint				mUniformMesh_PointLights_Color[4];
	GLint				mUniformMesh_PointLights_AmbientIntensity[4];
	GLint				mUniformMesh_PointLights_DiffuseIntensity[4];
	GLint				mUniformMesh_PointLights_Attenuation[4];
	GLint				mUniformMesh_HasTexture;
	GLint				mUniformSilhouette_EdgeWidth;
	GLint				mUniformSilhouette_MVP;
	GLint				mUniformSilhouette_Alpha;
	GLint				mUniformShadow_MVP;

};