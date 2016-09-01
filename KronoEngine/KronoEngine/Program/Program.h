#pragma once

#include <GL\glew.h>
#include <string>
#include <map>
#include <glm\glm.hpp>

namespace GLSLShader {
	enum GLSLShaderType {
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER
	};
}

class Program {
public:

								Program();
								~Program();

	void						CompileShader(const char *path, GLSLShader::GLSLShaderType type);
	void						CompileShader(const std::string &source, GLSLShader::GLSLShaderType type, const char *path = NULL);

	void						Link();
	void						Use();
	void						StopUsing();

	int							GetHandle();
	bool						IsLinked();

	void						BindAttributeLocation(GLuint location, const char *name);
	void						BindFragmentDataLocation(GLuint location, const char *name);

	void						SetUniform(const char *name, float x, float y, float z);
	void						SetUniform(const GLint location, float x, float y, float z);
	void						SetUniform(const char *name, float x, float y, float z, float w);
	void						SetUniform(const GLint location, float x, float y, float z, float w);
	void						SetUniform(const char *name, const glm::vec2 &vector);
	void						SetUniform(const GLint location, const glm::vec2 &vector);
	void						SetUniform(const char *name, const glm::vec3 &vector);
	void						SetUniform(const GLint location, const glm::vec3 &vector);
	void						SetUniform(const char *name, const glm::vec4 &vector);
	void						SetUniform(const GLint location, const glm::vec4 &vector);
	void						SetUniform(const char *name, const glm::mat3 &matrix);
	void						SetUniform(const GLint location, const glm::mat3 &matrix);
	void						SetUniform(const char *name, const glm::mat4 &matrix);
	void						SetUniform(const GLint location, const glm::mat4 &matrix);
	void						SetUniform(const char *name, float value);
	void						SetUniform(const GLint location, float value);
	void						SetUniform(const char *name, int value);
	void						SetUniform(const GLint location, int value);
	void						SetUniform(const char *name, bool value);
	void						SetUniform(const GLint location, bool value);
	void						SetUniform(const char *name, GLuint value);
	void						SetUniform(const GLint location, GLuint value);

	GLint						GetUniformLocation(const char *name);

	void						PrintActiveUniforms();
	void						PrintActiveAttributes();

	const char					*GetTypeString(GLenum type);

private:

	int							mHandle;
	bool						mLinked;
	std::map<std::string, int>	mUniformLocations;

	bool						FileExists(const std::string &fileName);

};