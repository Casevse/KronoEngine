#include <Program\Program.h>

#include <iostream>
#include <fstream>
#include <sstream>

Program::Program() 
	: mHandle(0)
	, mLinked(false) {

}

Program::~Program() {
	if (mHandle == 0) return;

	GLint numShaders = 0;
	glGetProgramiv(mHandle, GL_ATTACHED_SHADERS, &numShaders);

	GLuint *shaderNames = new GLuint[numShaders];
	glGetAttachedShaders(mHandle, numShaders, NULL, shaderNames);

	for (int i = 0; i < numShaders; i++) {
		glDeleteShader(shaderNames[i]);
	}

	glDeleteProgram(mHandle);

	delete[] shaderNames;
}

void Program::CompileShader(const char *path, GLSLShader::GLSLShaderType type) {
	if (!FileExists(path)) {
		std::cout << "ERROR: " << path << " not found\n";
		return;
	}

	if (mHandle <= 0) {
		mHandle = glCreateProgram();
		if (mHandle == 0) {
			std::cout << "ERROR: Can't create shader program\n";
			return;
		}
	}

	std::ifstream inFile(path, std::ios::in);
	if (!inFile) {
		std::cout << "ERROR: Can't open: " << path << std::endl;
		return;
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();

	CompileShader(code.str(), type, path);
}

void Program::CompileShader(const std::string &source, GLSLShader::GLSLShaderType type, const char *path) {
	if (mHandle <= 0) {
		mHandle = glCreateProgram();
		if (mHandle == 0) {
			std::cout << "ERROR: Can't create shader program\n";
			return;
		}
	}

	GLuint shaderHandle = glCreateShader(type);
	const char *code = source.c_str();

	glShaderSource(shaderHandle, 1, &code, NULL);
	glCompileShader(shaderHandle);

	// Check errors.
	int result;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result) {
		int length = 0;
		std::string logString;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char *log = new char[length];
			int written = 0;
			glGetShaderInfoLog(shaderHandle, length, &written, log);
			logString = log;
			delete[] log;
		}

		std::string message;
		if (path) {
			message = std::string(path) + ": shader compilation failed\n";
		} else {
			message = "Shader compilation failed\n";
		}
		message += logString;
	} else {
		glAttachShader(mHandle, shaderHandle);
	}
}

void Program::Link() {
	if (mLinked) return;
	if (mHandle <= 0) return;
	glLinkProgram(mHandle);

	int status = 0;
	glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		int length = 0;
		std::string logString;

		glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			char * log = new char[length];
			int written = 0;
			glGetProgramInfoLog(mHandle, length, &written, log);
			logString = log;
			delete[] log;
		}

		std::cout << logString << std::endl;
	}
	else {
		mUniformLocations.clear();
		mLinked = true;
	}
}

void Program::Use() {
	if (mHandle <= 0 || (!mLinked)) return;
	glUseProgram(mHandle);
}

void Program::StopUsing() {
	glUseProgram(0);
}

int Program::GetHandle() {
	return mHandle;
}

bool Program::IsLinked() {
	return mLinked;
}

void Program::BindAttributeLocation(GLuint location, const char *name) {
	glBindAttribLocation(mHandle, location, name);
}

void Program::BindFragmentDataLocation(GLuint location, const char *name) {
	glBindFragDataLocation(mHandle, location, name);
}

GLint Program::GetUniformLocation(const char *name) {
	std::map<std::string, int>::iterator position;
	position = mUniformLocations.find(name);

	if (position == mUniformLocations.end()) {
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);
	}

	return mUniformLocations[name];
}

void Program::SetUniform(const char *name, float x, float y, float z) {
	GLint location = GetUniformLocation(name);
	glUniform3f(location, x, y, z);
}

void Program::SetUniform(const GLint location, float x, float y, float z) {
	glUniform3f(location, x, y, z);
}

void Program::SetUniform(const char *name, float x, float y, float z, float w) {
	GLint location = GetUniformLocation(name);
	glUniform4f(location, x, y, z, w);
}

void Program::SetUniform(const GLint location, float x, float y, float z, float w) {
	glUniform4f(location, x, y, z, w);
}

void Program::SetUniform(const char *name, const glm::vec2 &vector) {
	GLint location = GetUniformLocation(name);
	glUniform2f(location, vector.x, vector.y);
}

void Program::SetUniform(const GLint location, const glm::vec2 &vector) {
	glUniform2f(location, vector.x, vector.y);
}

void Program::SetUniform(const char *name, const glm::vec3 &vector) {
	this->SetUniform(name, vector.x, vector.y, vector.z);
}

void Program::SetUniform(const GLint location, const glm::vec3 &vector) {
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void Program::SetUniform(const char *name, const glm::vec4 &vector) {
	GLint location = GetUniformLocation(name);
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void Program::SetUniform(const GLint location, const glm::vec4 &vector) {
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void Program::SetUniform(const char *name, const glm::mat3 &matrix) {
	GLint location = GetUniformLocation(name);
	glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Program::SetUniform(const GLint location, const glm::mat3 &matrix) {
	glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Program::SetUniform(const char *name, const glm::mat4 &matrix) {
	GLint location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Program::SetUniform(const GLint location, const glm::mat4 &matrix) {
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Program::SetUniform(const char *name, float value) {
	GLint location = GetUniformLocation(name);
	glUniform1f(location, value);
}

void Program::SetUniform(const GLint location, float value) {
	glUniform1f(location, value);
}

void Program::SetUniform(const char *name, int value) {
	GLint location = GetUniformLocation(name);
	glUniform1i(location, value);
}

void Program::SetUniform(const GLint location, int value) {
	glUniform1i(location, value);
}

void Program::SetUniform(const char *name, GLuint value) {
	GLint location = GetUniformLocation(name);
	glUniform1ui(location, value);
}

void Program::SetUniform(const GLint location, GLuint value) {
	glUniform1ui(location, value);
}

void Program::SetUniform(const char *name, bool value) {
	GLint lcation = GetUniformLocation(name);
	glUniform1i(lcation, value);
}

void Program::SetUniform(const GLint location, bool value) {
	glUniform1i(location, value);
}

void Program::PrintActiveUniforms() {
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(mHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

	std::cout << "Active Uniforms:\n";
	for (int i = 0; i < numUniforms; ++i) {
		GLint results[4];
		glGetProgramResourceiv(mHandle, GL_UNIFORM, i, 4, properties, 4, NULL, results);

		if (results[3] != -1) continue;
		GLint nameBufferSize = results[0] + 1;
		char *name = new char[nameBufferSize];
		glGetProgramResourceName(mHandle, GL_UNIFORM, i, nameBufferSize, NULL, name);

		std::cout << results[2] << " - " << name << " - " << GetTypeString(results[1]) << std::endl;

		delete[] name;
	}
}

void Program::PrintActiveAttributes() {
	GLint numAttributes;
	glGetProgramInterfaceiv(mHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttributes);

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

	std::cout << "Active Attributes:\n";
	for (int i = 0; i < numAttributes; ++i) {
		GLint results[3];
		glGetProgramResourceiv(mHandle, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

		GLint nameBufferSize = results[0] + 1;
		char *name = new char[nameBufferSize];
		glGetProgramResourceName(mHandle, GL_PROGRAM_INPUT, i, nameBufferSize, NULL, name);

		std::cout << results[2] << " - " << name << " - " << GetTypeString(results[1]) << std::endl;

		delete[] name;
	}
}

const char *Program::GetTypeString(GLenum type) {
	switch (type) {
	case GL_FLOAT:			return "float";
	case GL_FLOAT_VEC2:		return "vec2";
	case GL_FLOAT_VEC3:		return "vec3";
	case GL_FLOAT_VEC4:		return "vec4";
	case GL_DOUBLE:			return "double";
	case GL_INT:			return "int";
	case GL_UNSIGNED_INT:	return "unsigned int";
	case GL_BOOL:			return "bool";
	case GL_FLOAT_MAT2:		return "mat2";
	case GL_FLOAT_MAT3:		return "mat3";
	case GL_FLOAT_MAT4:		return "mat4";
	default:				return "?";
	}
}

bool Program::FileExists(const std::string &fileName) {
	struct stat info;
	int ret = -1;

	ret = stat(fileName.c_str(), &info);

	return 0 == ret;
}