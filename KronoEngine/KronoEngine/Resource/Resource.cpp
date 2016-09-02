#include <Resource\Resource.h>

void Resource::SetAlias(char *alias) {
	mAlias = alias;
}

std::string Resource::GetAlias() {
	return mAlias;
}

std::string Resource::GetPath() {
	return mPath;
}