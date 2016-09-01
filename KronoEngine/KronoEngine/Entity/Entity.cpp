#include <Entity\Entity.h>

Entity::Entity() {
	mPlaneCache = -1;
}

void Entity::SetType(EntityTypes type) {
	mType = type;
}

int Entity::GetType() {
	return mType;
}

bool Entity::CheckFrustumCulling(const glm::mat4 &model, const bool moved, const bool frameChanged) {
	if (moved || frameChanged) {
		if (mStatic) {
			mVertices[0] = glm::vec4(mMin.x, mMin.y, mMin.z, mMax.w);
			mVertices[1] = glm::vec4(mMin.x, mMin.y, mMax.z, mMax.w);
			mVertices[2] = glm::vec4(mMax.x, mMin.y, mMax.z, mMax.w);
			mVertices[3] = glm::vec4(mMax.x, mMin.y, mMin.z, mMax.w);
			mVertices[4] = glm::vec4(mMin.x, mMax.y, mMin.z, mMax.w);
			mVertices[5] = glm::vec4(mMin.x, mMax.y, mMax.z, mMax.w);
			mVertices[6] = glm::vec4(mMax.x, mMax.y, mMax.z, mMax.w);
			mVertices[7] = glm::vec4(mMax.x, mMax.y, mMin.z, mMax.w);
		} else {
			mVertices[0] = model * glm::vec4(-mMin.x, mMin.y, -mMin.z, 1.0f);
			mVertices[1] = model * glm::vec4(-mMin.x, mMin.y, -mMax.z, 1.0f);
			mVertices[2] = model * glm::vec4(-mMax.x, mMin.y, -mMax.z, 1.0f);
			mVertices[3] = model * glm::vec4(-mMax.x, mMin.y, -mMin.z, 1.0f);
			mVertices[4] = model * glm::vec4(-mMin.x, mMax.y, -mMin.z, 1.0f);
			mVertices[5] = model * glm::vec4(-mMin.x, mMax.y, -mMax.z, 1.0f);
			mVertices[6] = model * glm::vec4(-mMax.x, mMax.y, -mMax.z, 1.0f);
			mVertices[7] = model * glm::vec4(-mMax.x, mMax.y, -mMin.z, 1.0f);
		}
	}

	glm::vec4 *plane = mProgramManager->GetFrustumPlanes();
	ClipResult result = INSIDE;
	int out, in;

	if (mPlaneCache != -1) {
		out = 0;
		in = 0;

		for (int j = 0; j < 8; ++j) {
			if (glm::dot(*(plane + mPlaneCache), mVertices[j]) < 0.0f) {
				++out;
			} else {
				++in;
			}

			if (!in) {
				result = OUTSIDE;
			} else if (out > 0) {
				result = INTERSECT;
			}
		}
	}

	if (result != OUTSIDE) {
		for (int i = 0; i < 6; ++i) {
			if (result == OUTSIDE) {
				break;
			}

			if (mPlaneCache == i) {
				continue;
			}

			out = 0;
			in = 0;

			for (int j = 0; j < 8; ++j) {
				if (glm::dot(*(plane + i), mVertices[j]) < 0.0f) {
					++out;
				} else {
					++in;
				}
			}

			if (!in) {
				result = OUTSIDE;
				mPlaneCache = i;
				break;
			} else if (out > 0) {
				result = INTERSECT;
			}
		}
	}

	if (result == INSIDE || result == INTERSECT) {
		mPlaneCache = -1;
		return true;
	} else {
		return false;
	}
}