#pragma once

#include <Resource\Resource.h>
#include <GL\glew.h>

class TextureResource : public Resource {
public:

				TextureResource(char *path, bool flipped = false);
				~TextureResource();

	void		LoadTexture(char *path);

	GLuint		GetTextureObject() const;
	GLfloat		GetOriginalWidth() const;
	GLfloat		GetOriginalHeight() const;

	void		SetRepeat(const bool repeat);

private:

	GLuint		mTextureObject;
	GLfloat		mOriginalWidth;
	GLfloat		mOriginalHeight;

	GLint		mMinMagFilter;
	GLint		mWrapMode;

};