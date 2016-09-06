#include <Resource\TextureResource.h>

#include <lodepng\lodepng.h>

TextureResource::TextureResource(char *path, bool flipped) {
	mPath = path;

	std::vector<unsigned char> image;
	unsigned int width, height, error;
	error = lodepng::decode(image, width, height, path);

	// Flip Y
	unsigned char* pixels = (unsigned char*)malloc(width * height * 4);
	unsigned int pos = 0;

	for (unsigned int r = 0; r < height; r++) {
		for (unsigned int b = 0; b < width * 4; b++) {
			pixels[pos] = image[(height - r - 1) * width * 4 + b];
			pos++;
		}
	}

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
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
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