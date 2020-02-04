#pragma once

class GLSLProgram
{
public:
	void CompileShadersFromFile(std::string_view vertexShaderFilePath, std::string_view fragmentShaderFilePath);
	void CompileShadersFromSource(std::string_view vertexSource, std::string_view fragmentSource);

	void AddAttribute(std::string_view attributeName);

	GLint GetUniformLocation(std::string_view uniformName);

	void LinkShaders();

	void Use();
	void Unuse();

	void Dispose();

private:
	void compileShaders(const char *source, std::string_view name, const GLuint &id);

	int m_numAttributtes = 0;
	GLuint m_programID = 0;	
	GLuint m_vertexShaderID = 0;
	GLuint m_fragmentShaderID = 0;
};