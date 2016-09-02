#pragma once

#include <Program\ProgramManager.h>
#include <Resource\MeshResource.h>

class ResourceManager {
public:

										ResourceManager(ProgramManager *programManager);
										~ResourceManager();

	void								Clear();

	MeshResource						*GetMesh(char *path, char *alias = NULL);
	void								RemoveMesh(char *path, char *alias = NULL);

	TextureResource						*GetTexture(char *path, bool flipped = false, char *alias = NULL);
	TextureResource						*GetTexture(char *path, unsigned char *pixels, int width, int height);
	void								RemoveTexture(char *path, char *alias = NULL);

	void								ShowMeshes();
	void								ShowTextures();
	void								ShowAll();

private:

	ProgramManager						*mProgramManager;

	std::vector<MeshResource*>			mMeshes;
	std::vector<TextureResource*>		mTextures;

};