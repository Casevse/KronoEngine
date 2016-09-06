#include <Resource\MeshResource.h>

#include <fstream>
#include <Config.h>

MeshResource::MeshResource(char *path, ProgramManager *programManager) {
	mPath = path;
	SetAlias("");

	mProgramManager = programManager;

	LoadKMBin(path);

	mCenterPoint = glm::vec4((mMaxPoint.x + mMinPoint.x) / 2.0f, (mMaxPoint.y + mMinPoint.y) / 2.0f, (mMaxPoint.z + mMinPoint.z) / 2.0f, 1.0f);
	mRadius = glm::distance(mMaxPoint, mMinPoint) / 2.0f;

	mProgramMesh = mProgramManager->Get("ToonMesh");
	mProgramSilhouette = mProgramManager->Get("ToonSilhouette");
	mProgramShadowMap = mProgramManager->Get("ShadowMap");

	mTexture = NULL;

	// Uniforms Mesh
	mUniformMesh_VertexColor = mProgramMesh->GetUniformLocation("VertexColor");
	mUniformMesh_ModelViewMatrix = mProgramMesh->GetUniformLocation("ModelViewMatrix");
	mUniformMesh_NormalMatrix = mProgramMesh->GetUniformLocation("NormalMatrix");
	mUniformMesh_MVP = mProgramMesh->GetUniformLocation("MVP");
	mUniformMesh_HasShadowMap = mProgramMesh->GetUniformLocation("HasShadowMap");
	mUniformMesh_ShadowMatrix = mProgramMesh->GetUniformLocation("ShadowMatrix");
	mUniformMesh_ShadowMap = mProgramMesh->GetUniformLocation("ShadowMap");
	mUniformMesh_SunPosition = mProgramMesh->GetUniformLocation("SunPosition");
	mUniformMesh_SunIntensity = mProgramMesh->GetUniformLocation("SunIntensity");
	mUniformMesh_SunColor = mProgramMesh->GetUniformLocation("SunColor");
	mUniformMesh_NumPointLights = mProgramMesh->GetUniformLocation("NumPointLights");
	for (int i = 0; i < 4; i++) {	// Same as the shader.
		std::string indexStr = std::to_string(i);

		mUniformMesh_PointLights_Position[i] = mProgramMesh->GetUniformLocation(std::string("PointLights[" + indexStr + "].Position").c_str());
		mUniformMesh_PointLights_Color[i] = mProgramMesh->GetUniformLocation(std::string("PointLights[" + indexStr + "].Color").c_str());
		mUniformMesh_PointLights_AmbientIntensity[i] = mProgramMesh->GetUniformLocation(std::string("PointLights[" + indexStr + "].AmbientIntensity").c_str());
		mUniformMesh_PointLights_DiffuseIntensity[i] = mProgramMesh->GetUniformLocation(std::string("PointLights[" + indexStr + "].DiffuseIntensity").c_str());
		mUniformMesh_PointLights_Attenuation[i] = mProgramMesh->GetUniformLocation(std::string("PointLights[" + indexStr + "].Attenuation").c_str());
	}
	mUniformMesh_HasTexture = mProgramMesh->GetUniformLocation("HasTexture");

	// Uniform Silhouette
	mProgramSilhouette->Use();
	mUniformSilhouette_EdgeWidth = mProgramSilhouette->GetUniformLocation("EdgeWidth");
	mUniformSilhouette_MVP = mProgramSilhouette->GetUniformLocation("MVP");
	mUniformSilhouette_Alpha = mProgramSilhouette->GetUniformLocation("Alpha");
	mProgramSilhouette->SetUniform(mUniformSilhouette_EdgeWidth, 0.005f);

	// Uniforms ShadowMap
	mUniformShadow_MVP = mProgramShadowMap->GetUniformLocation("MVP");
}

MeshResource::~MeshResource() {
	glDeleteVertexArrays(1, &mVaoHandle);
	glDeleteVertexArrays(1, &mVaoHandleSilhouette);
	glDeleteBuffers(1, &mHandle[0]);
	glDeleteBuffers(1, &mHandle[1]);
	glDeleteBuffers(1, &mHandle[2]);
	glDeleteBuffers(1, &mHandle[3]);
	glDeleteBuffers(1, &mHandleSilhouette[0]);
	glDeleteBuffers(1, &mHandleSilhouette[1]);
}

void MeshResource::Draw(const glm::mat4 &model, const glm::mat4 &modelView, const glm::mat3 &normalMatrix, const glm::mat4 &modelViewProjection, TextureResource *texture, const glm::vec4 &color) {
	if (mTexture != texture) {
		mTexture = texture;
	}

	mProgramMesh->Use();

	mProgramMesh->SetUniform(mUniformMesh_VertexColor, color);
	mProgramMesh->SetUniform(mUniformMesh_ModelViewMatrix, modelView);
	mProgramMesh->SetUniform(mUniformMesh_NormalMatrix, normalMatrix);
	mProgramMesh->SetUniform(mUniformMesh_MVP, modelViewProjection);

	if (Config::shadows) {
		mProgramMesh->SetUniform(mUniformMesh_HasShadowMap, 1.0f);
		mProgramMesh->SetUniform(mUniformMesh_ShadowMatrix, mProgramManager->GetGlobalLightVPB() * model);
		mProgramMesh->SetUniform(mUniformMesh_ShadowMap, 1);
	} else {
		mProgramMesh->SetUniform(mUniformMesh_HasShadowMap, 0.0f);
	}

	GlobalLight globalLight = mProgramManager->GetGlobalLightParameters();
	mProgramMesh->SetUniform(mUniformMesh_SunPosition, mProgramManager->GetActiveCameraView() * globalLight.mPosition);
	mProgramMesh->SetUniform(mUniformMesh_SunIntensity, globalLight.mIntensity);
	mProgramMesh->SetUniform(mUniformMesh_SunColor, globalLight.mColor);

	std::vector<PointLight> pointLights = mProgramManager->GetActiveLights();
	mProgramMesh->SetUniform(mUniformMesh_NumPointLights, (int)pointLights.size());
	for (unsigned int i = 0; i < pointLights.size(); i++) {
		mProgramMesh->SetUniform(mUniformMesh_PointLights_Position[i], glm::vec3(mProgramManager->GetActiveCameraView() * pointLights[i].mPosition));
		mProgramMesh->SetUniform(mUniformMesh_PointLights_Color[i], pointLights[i].mColor);
		mProgramMesh->SetUniform(mUniformMesh_PointLights_AmbientIntensity[i], pointLights[i].mAmbientIntensity);
		mProgramMesh->SetUniform(mUniformMesh_PointLights_DiffuseIntensity[i], pointLights[i].mDiffuseIntensity);
		mProgramMesh->SetUniform(mUniformMesh_PointLights_Attenuation[i], pointLights[i].mAttenuation);
	}

	if (mTexture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexture->GetTextureObject());
		mProgramMesh->SetUniform(mUniformMesh_HasTexture, 1.0f);
	} else {
		mProgramMesh->SetUniform(mUniformMesh_HasTexture, 0.0f);
	}

	glBindVertexArray(mVaoHandle);
	glDrawElements(GL_TRIANGLES_ADJACENCY, mNumFaces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));

	if (mTexture) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Silhouette.
	mProgramSilhouette->Use();
	mProgramSilhouette->SetUniform(mUniformSilhouette_MVP, modelViewProjection);
	mProgramSilhouette->SetUniform(mUniformSilhouette_Alpha, color.a);

	glBindVertexArray(mVaoHandleSilhouette);
	glDrawElements(GL_TRIANGLES_ADJACENCY, mNumFaces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
	glBindVertexArray(0);
}

void MeshResource::DrawShadowMap(const glm::mat4 &model) {
	mProgramShadowMap->Use();
	mProgramShadowMap->SetUniform(mUniformShadow_MVP, mProgramManager->GetGlobalLightProjection() * mProgramManager->GetGlobalLightView() * model);

	glBindVertexArray(mVaoHandle);
	glDrawElements(GL_TRIANGLES_ADJACENCY, mNumFaces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
	glBindVertexArray(0);
}

const glm::vec3 &MeshResource::GetMaxPoint() const {
	return mMaxPoint;
}

const glm::vec3 &MeshResource::GetMinPoint() const {
	return mMinPoint;
}

const glm::vec4 &MeshResource::GetCenterPoint() const {
	return mCenterPoint;
}

const float MeshResource::GetRadius() const {
	return mRadius;
}

void MeshResource::LoadKMBin(char *path) {
	std::ifstream stream(path, std::ios::binary);

	int numVertices, numVerticesSilhouette;

	stream.read((char *)&numVertices, sizeof(int));
	stream.read((char *)&numVerticesSilhouette, sizeof(int));
	stream.read((char *)&mNumFaces, sizeof(unsigned int));

	stream.read((char *)&mMaxPoint.x, sizeof(float));
	stream.read((char *)&mMaxPoint.y, sizeof(float));
	stream.read((char *)&mMaxPoint.z, sizeof(float));
	stream.read((char *)&mMinPoint.x, sizeof(float));
	stream.read((char *)&mMinPoint.y, sizeof(float));
	stream.read((char *)&mMinPoint.z, sizeof(float));

	float *v = new float[numVertices * 3];
	stream.read((char *)v, numVertices * 3 * sizeof(float));

	float *n = new float[numVertices * 3];
	stream.read((char *)n, numVertices * 3 * sizeof(float));

	float *tc = new float[numVertices * 2];
	stream.read((char *)tc, numVertices * 2 * sizeof(float));

	unsigned int *el = new unsigned int[mNumFaces];
	stream.read((char *)el, mNumFaces * sizeof(unsigned int));

	float *vs = new float[numVerticesSilhouette * 3];
	stream.read((char *)vs, numVerticesSilhouette * 3 * sizeof(float));

	unsigned int *els = new unsigned int[mNumFaces];
	stream.read((char *)els, mNumFaces * sizeof(unsigned int));

	stream.close();

	// Store VBO.

	// Mesh.
	glGenVertexArrays(1, &mVaoHandle);
	glBindVertexArray(mVaoHandle);

	int numBuffers = 4;
	GLuint elementBuffer = 3;
	mHandle[4];

	glGenBuffers(numBuffers, mHandle);

	glBindBuffer(GL_ARRAY_BUFFER, mHandle[0]);
	glBufferData(GL_ARRAY_BUFFER, (3 * numVertices) * sizeof(float), v, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(0);	// Vertices

	glBindBuffer(GL_ARRAY_BUFFER, mHandle[1]);
	glBufferData(GL_ARRAY_BUFFER, (2 * numVertices) * sizeof(float), tc, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);	// Textures

	glBindBuffer(GL_ARRAY_BUFFER, mHandle[2]);
	glBufferData(GL_ARRAY_BUFFER, (3 * numVertices) * sizeof(float), n, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(2);	// Normals

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle[elementBuffer]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumFaces * sizeof(unsigned int), el, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete[] v;
	delete[] n;
	delete[] tc;
	delete[] el;

	// Silhouette.
	glGenVertexArrays(1, &mVaoHandleSilhouette);
	glBindVertexArray(mVaoHandleSilhouette);

	int numBuffersSilhouette = 2;
	GLuint elementBufferSilhouette = 1;
	mHandleSilhouette[2];

	glGenBuffers(numBuffersSilhouette, mHandleSilhouette);

	glBindBuffer(GL_ARRAY_BUFFER, mHandleSilhouette[0]);
	glBufferData(GL_ARRAY_BUFFER, (3 * numVerticesSilhouette) * sizeof(float), vs, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(0);	// Vertices

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandleSilhouette[elementBufferSilhouette]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumFaces * sizeof(unsigned int), els, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete[] vs;
	delete[] els;
}