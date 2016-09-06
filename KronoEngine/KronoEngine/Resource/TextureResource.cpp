#include <Resource\TextureResource.h>

TextureResource::TextureResource(char *path, bool flipped) {
	mPath = path;

	// TODO load from image using some library.
}

TextureResource::TextureResource(char *path, unsigned char *pixels, int width, int height) {
	mPath = path;

	mMinMagFilter = GL_LINEAR;
	mWrapMode = GL_CLAMP_TO_EDGE;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &mTextureObject);
	glBindTexture(GL_TEXTURE_2D, mTextureObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMinMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapMode);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		(GLsizei)width,
		(GLsizei)height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
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