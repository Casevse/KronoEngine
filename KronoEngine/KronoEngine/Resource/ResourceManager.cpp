#include <Resource\ResourceManager.h>

#include <iostream>

ResourceManager::ResourceManager(ProgramManager *programManager) {
	mProgramManager = programManager;
	mMeshes.resize(0);
	mTextures.resize(0);
}

ResourceManager::~ResourceManager() {
	Clear();
}

void ResourceManager::Clear() {
	for (unsigned int i = 0; i < mMeshes.size(); i++) {
		delete mMeshes[i];
	}
	mMeshes.clear();
	for (unsigned int i = 0; i < mTextures.size(); i++) {
		delete mTextures[i];
	}
	mTextures.clear();
}

MeshResource *ResourceManager::GetMesh(char *path, char *alias) {
	MeshResource *mesh = NULL;
	for (unsigned int i = 0; i < mMeshes.size(); i++) {
		if (alias) {
			if (mMeshes[i]->GetAlias().compare(alias) == 0) {
				mesh = mMeshes[i];
				break;
			}
		} else {
			if (mMeshes[i]->GetPath().compare(path) == 0) {
				mesh = mMeshes[i];
				break;
			}
		}
	}

	if (!mesh) {
		mesh = new MeshResource(path, mProgramManager);
		if (alias) {
			mesh->SetAlias(alias);
		}
		mMeshes.push_back(mesh);
	}

	return mesh;
}

void ResourceManager::RemoveMesh(char *path, char *alias) {
	for (unsigned int i = 0; i < mMeshes.size(); i++) {
		if (alias) {
			if (!mMeshes[i]->GetAlias().empty()) {
				if (mMeshes[i]->GetAlias().compare(alias) == 0) {
					delete mMeshes[i];
					mMeshes.erase(mMeshes.begin() + i);
					break;
				}
			}
		} else {
			if (mMeshes[i]->GetPath().compare(path) == 0) {
				delete mMeshes[i];
				mMeshes.erase(mMeshes.begin() + i);
				break;
			}
		}
	}
}

TextureResource *ResourceManager::GetTexture(char *path, bool flipped, char *alias) {
	TextureResource *texture = NULL;
	for (unsigned int i = 0; i < mTextures.size(); i++) {
		if (alias) {
			if (mTextures[i]->GetAlias().compare(alias) == 0) {
				texture = mTextures[i];
				break;
			}
		} else {

			if (mTextures[i]->GetPath().compare(path) == 0) {
				texture = mTextures[i];
				break;
			}
		}
	}

	if (!texture) {
		texture = new TextureResource(path, flipped);
		if (alias) {
			texture->SetAlias(alias);
		}
		mTextures.push_back(texture);
	}

	return texture;
}

TextureResource *ResourceManager::GetTexture(char *path, unsigned char *pixels, int width, int height) {
	TextureResource *texture = NULL;
	for (unsigned int i = 0; i < mTextures.size(); i++) {
		if (mTextures[i]->GetPath().compare(path) == 0) {
			texture = mTextures[i];
			break;
		}
	}

	if (!texture) {
		texture = new TextureResource(path, pixels, width, height);
		mTextures.push_back(texture);
	}

	return texture;
}

void ResourceManager::RemoveTexture(char *path, char *alias) {
	for (unsigned int i = 0; i < mTextures.size(); i++) {
		if (alias) {
			if (!mTextures[i]->GetAlias().empty()) {
				if (mTextures[i]->GetAlias().compare(alias) == 0) {
					delete mTextures[i];
					mTextures.erase(mTextures.begin() + i);
					break;
				}
			}
		} else {
			if (mTextures[i]->GetPath().compare(path) == 0) {
				delete mTextures[i];
				mTextures.erase(mTextures.begin() + i);
				break;
			}
		}
	}
}

void ResourceManager::ShowMeshes() {
	std::cout << "Meshes\n------------\n";
	for (unsigned int i = 0; i < mMeshes.size(); i++) {
		std::cout << mMeshes[i]->GetPath() << std::endl;
	}
	std::cout << std::endl;
}

void ResourceManager::ShowTextures() {
	std::cout << "Textures\n------------\n";
	for (unsigned int i = 0; i < mTextures.size(); i++) {
		std::cout << mTextures[i]->GetPath() << std::endl;
	}
	std::cout << std::endl;
}

void ResourceManager::ShowAll() {
	ShowMeshes();
	ShowTextures();
}