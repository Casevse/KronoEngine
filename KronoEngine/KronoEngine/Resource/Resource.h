#pragma once

#include <string>

class Resource {
public:

	virtual			~Resource() {};

	void			SetAlias(char *alias);
	std::string		GetAlias();

	std::string		GetPath();

protected:

	std::string		mPath;

private:

	std::string		mAlias;

};