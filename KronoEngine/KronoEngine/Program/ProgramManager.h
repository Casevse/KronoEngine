#pragma once

#include <glm\glm.hpp>
#include <string>
#include <Program\Program.h>
#include <vector>

struct GlobalLight {
	glm::vec4	mPosition;
	float		mIntensity;
	glm::vec3	mColor;
};

struct PointLight {
	glm::vec4	mPosition;
	glm::vec3	mColor;
	float		mAmbientIntensity;
	float		mDiffuseIntensity;
	float		mAttenuation;
};

class ProgramManager {
public:

										ProgramManager();
										~ProgramManager();

	void								Add(const std::string &name, Program *program);
	void								Remove(const std::string &name);
	void								Clear();

	Program								*Get(const std::string &name);

	void								SetActiveCameraMatrix(const glm::mat4 &matrix);
	glm::mat4							GetActiveCameraMatrix();

	void								SetActiveCameraView(const glm::mat4 &view);
	glm::mat4							GetActiveCameraView();
	const bool							GetActiveCameraViewChanged();

	void								SetActiveCameraProjection(const glm::mat4 &projection);
	glm::mat4							GetActiveCameraProjection();
	const bool							GetActiveCameraProjectionChanged();

	void								SetActiveCameraOrientation(const glm::mat4 &orientation);
	glm::mat4							GetActiveCameraOrientation();

	void								SetActiveCameraPosition(const glm::vec3 &position);
	glm::vec3							GetActiveCameraPosition();

	void								ResetChangeFlags();

	void								CalculateFrustumPlanes();
	glm::vec4							*GetFrustumPlanes();

	void								InitPrograms();

	void								SetGlobalLightParameters(const glm::vec3 &position, const float intensity, const glm::ivec4 &color, const float offset);
	GlobalLight							&GetGlobalLightParameters();

	void								AddActiveLight(const glm::vec4 &position, const glm::vec3 &color, const float ambientIntensity, const float diffuseIntensity, const float attenuation);
	std::vector<PointLight>				&GetActiveLights();
	void								ClearActiveLights();

	// Shadow Map
	glm::mat4							&GetGlobalLightView();
	glm::mat4							&GetGlobalLightProjection();
	glm::mat4							&GetGlobalLightVPB();

private:

	std::map<std::string, Program*>		mPrograms;

	glm::mat4							mActiveCameraMatrix;
	glm::mat4							mActiveCameraView;
	bool								mActiveCameraViewChanged;
	glm::mat4							mActiveCameraProjection;
	bool								mActiveCameraProjectionChanged;
	glm::mat4							mActiveCameraOrientation;
	glm::vec3							mActiveCameraPosition;

	glm::vec4							mFrustumPlanes[6];

	GlobalLight							mGlobalLight;
	std::vector<PointLight>				mActiveLights;

	glm::mat4							mGlobalLightView;
	glm::mat4							mGlobalLightProjection;
	glm::mat4							mGlobalLightVPB;
	glm::mat4							mBias;	// For calculating Shadow Map.

};