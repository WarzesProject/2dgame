#pragma once

class GLSLProgram
{
public:
	void CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	void CompileShadersFromSource(const char* vertexSource, const char* fragmentSource);

	void AddAttribute(const std::string& attributeName);

	GLint GetUniformLocation(const std::string& uniformName);

	void LinkShaders();

	void Use();
	void Unuse();

	void Dispose();

private:
	void compileShaders(const char* source, const std::string& name, GLuint& id);

	int m_numAttributtes = 0;
	GLuint m_programID = 0;	
	GLuint m_vertexShaderID = 0;
	GLuint m_fragmentShaderID = 0;
};