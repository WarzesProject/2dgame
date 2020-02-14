#include "stdafx.h"
#include "ImageLoader.h"
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
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
//-----------------------------------------------------------------------------
Texture ImageLoader::Create(int width, int height)
{
	Texture texture;
	const int size = width * height * 4;
	std::vector<unsigned char> out(static_cast<size_t>(size));

	initGLTexture(texture.id, width, height, out);
	texture.width = width;
	texture.height = height;

	return texture;
}
//-----------------------------------------------------------------------------
Texture ImageLoader::Load(std::string_view filePath)
{
	Texture texture;

	std::vector<unsigned char> out;
	unsigned int width, height;
	auto errorCode = lodepng::decode(out, width, height, filePath.data());
	if ( errorCode != 0 )
		Throw("Decode PNG failed with error: " + std::to_string(errorCode) + "\n File : " + filePath.data());

	initGLTexture(texture.id, (GLsizei)width, (GLsizei)height, out);

	texture.width = (GLsizei)width;
	texture.height = (GLsizei)height;
	texture.filePath = filePath;

	return texture;
}
//-----------------------------------------------------------------------------