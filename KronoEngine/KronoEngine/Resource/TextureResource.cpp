#include <Resource\TextureResource.h>

TextureResource::TextureResource(char *path, bool flipped) {
	mPath = path;

	// TODO load from image using some library.
}

TextureResource::~TextureResource() {
	glDeleteTextures(1, &mTextureObject);
}

GLuint TextureResource::GetTextureObject() const {
	return mTextureObject;
}

GLfloat TextureResource::GetOriginalWidth() const {
	return mOriginalWidth;
}

GLfloat TextureResource::GetOriginalHeight() const {
	return mOriginalHeight;
}

void TextureResource::SetRepeat(const bool Repeat) {
	if (Repeat) {
		mWrapMode = GL_REPEAT;
	} else {
		mWrapMode = GL_CLAMP_TO_EDGE;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapMode);
	glBindTexture(GL_TEXTURE_2D, 0);
}