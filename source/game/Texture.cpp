#include "stdafx.h"
#include "Texture.h"
#include "Application.h"
#pragma warning( push, 1 )
#include "lodepng.cpp"
#pragma warning( pop )
//-----------------------------------------------------------------------------
void initGLTexture(GLuint &texid, GLsizei width, GLsizei height, std::vector<unsigned char> &out)
{
	glGenTextures(1, &(texid));
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if ( Application::Get()->GetConfig().graphics.textureFilter == TextureFilter::Point )
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
//-----------------------------------------------------------------------------
Texture::Texture(int width, int height)
{
	Create(width, height);
}
//-----------------------------------------------------------------------------
Texture::Texture(std::string_view filePath)
{
	Load(filePath);
}
//-----------------------------------------------------------------------------
void Texture::Create(int width, int height)
{
	const int size = width * height * 4;
	std::vector<unsigned char> out(static_cast<size_t>(size));
	initGLTexture(m_id, width, height, out);
	m_width = width;
	m_height = height;
}
//-----------------------------------------------------------------------------
void Texture::Load(std::string_view filePath)
{
	std::vector<unsigned char> out;
	unsigned int width, height;
	auto errorCode = lodepng::decode(out, width, height, filePath.data());
	if ( errorCode != 0 )
		Throw("Decode PNG failed with error: " + std::to_string(errorCode) + "\n File : " + filePath.data());

	initGLTexture(m_id, (GLsizei)width, (GLsizei)height, out);

	m_width = (GLsizei)width;
	m_height = (GLsizei)height;
	m_filePath = filePath;
}
//-----------------------------------------------------------------------------
void Texture::Close()
{
	glDeleteTextures(1, &m_id);
}
//-----------------------------------------------------------------------------