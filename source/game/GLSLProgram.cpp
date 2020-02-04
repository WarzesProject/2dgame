#include "stdafx.h"
#include "GLSLProgram.h"
#include "IOManager.h"
//-----------------------------------------------------------------------------
void GLSLProgram::CompileShadersFromFile(std::string_view vertexShaderFilePath, std::string_view fragmentShaderFilePath)
{
	std::string vertexSource;
	std::string fragSource;

	IOManager::ReadFileToBuffer(vertexShaderFilePath, vertexSource);
	IOManager::ReadFileToBuffer(fragmentShaderFilePath, fragSource);

	CompileShadersFromSource(vertexSource, fragSource);
}
//-----------------------------------------------------------------------------
void GLSLProgram::CompileShadersFromSource(std::string_view vertexSource, std::string_view fragmentSource)
{
	m_programID = glCreateProgram();

	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if ( m_vertexShaderID == 0 )
		Throw("Vertex shader failed to be created!");

	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if ( m_fragmentShaderID == 0 )
		Throw("Fragment shader failed to be created!");

	compileShaders(vertexSource.data(), "Vertex shader", m_vertexShaderID);
	compileShaders(fragmentSource.data(), "Fragment shader", m_fragmentShaderID);
}
//-----------------------------------------------------------------------------
void GLSLProgram::LinkShaders()
{
	//Attach our shaders to our program
	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);

	//Link our program
	glLinkProgram(m_programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
	if ( isLinked == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(m_programID);
		//Don't leak shaders either.
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);

		Throw(&(errorLog[0]) + std::string("\nShader failed to link!"));
	}

	//Always detach shaders after a successful link.
	glDetachShader(m_programID, m_vertexShaderID);
	glDetachShader(m_programID, m_fragmentShaderID);
	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);
}
//-----------------------------------------------------------------------------
void GLSLProgram::AddAttribute(std::string_view attributeName)
{
	glBindAttribLocation(m_programID, m_numAttributtes++, attributeName.data());
}
//-----------------------------------------------------------------------------
GLint GLSLProgram::GetUniformLocation(std::string_view uniformName)
{
	GLint location = glGetUniformLocation(m_programID, uniformName.data());
	if ( location == GL_INVALID_INDEX )
		Throw("Uniform " + std::string(uniformName.data()) + " not found in shader!");
	return location;
}
//-----------------------------------------------------------------------------
void GLSLProgram::Use()
{
	glUseProgram(m_programID);
	for ( int i = 0; i < m_numAttributtes; i++ )
		glEnableVertexAttribArray(i);
}
//-----------------------------------------------------------------------------
void GLSLProgram::Unuse()
{
	glUseProgram(0);
	for ( int i = 0; i < m_numAttributtes; i++ )
		glDisableVertexAttribArray(i);
}
//-----------------------------------------------------------------------------
void GLSLProgram::Dispose()
{
	if ( m_programID )
		glDeleteProgram(m_programID);

	m_numAttributtes = 0;
}
//-----------------------------------------------------------------------------
void GLSLProgram::compileShaders(const char* source, std::string_view name, const GLuint &id)
{
	glShaderSource(id, 1, &source, nullptr);

	glCompileShader(id);

	GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

	if ( isCompiled == GL_FALSE )
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(id); // Don't leak the shader.

		Throw(&(errorLog[0]) + std::string("Shader ") + name.data() + " failed to compile!");
	}
}
//-----------------------------------------------------------------------------