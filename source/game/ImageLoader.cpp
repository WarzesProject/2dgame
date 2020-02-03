#include "stdafx.h"
#include "ImageLoader.h"
#include "lodepng.cpp"
//-----------------------------------------------------------------------------
Texture ImageLoader::Create(GLsizei width, GLsizei height)
{
	Texture texture;
	const size_t size = width * height * 4;
	std::vector<unsigned char> out(size);

	TODO("настроить под рендеры. не забыть про текущий рендер из конфига");

	glGenTextures(1, &(texture.id));

	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;

	return texture;
}
//-----------------------------------------------------------------------------
Texture ImageLoader::Load(std::string_view filePath)
{
	Texture texture;

	std::vector<unsigned char> in;
	std::vector<unsigned char> out;

	unsigned int width, height;

	int errorCode = lodepng::decode(out, width, height, filePath.data());
	if ( errorCode != 0 )
		Throw("Decode PNG failed with error: " + std::to_string(errorCode) + "\n File : " + filePath.data());

	glGenTextures(1, &(texture.id));

	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;
	texture.filePath = filePath;

	return texture;
}
//-----------------------------------------------------------------------------
void ImageLoader::SavePNG(GLint x, GLint y, GLsizei width, GLsizei height, std::string_view filePath)
{
	std::vector<unsigned char> out(width * height * 4);

	glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	// reverse y coordinate
	for ( int j = 0, t = height - 1; j < (height / 2); j++, t-- )
	{
		for ( int i = 0; i < width; i++ )
		{
			unsigned char tmp[4];
			tmp[0] = out[4 * width * j + 4 * i + 0];
			tmp[1] = out[4 * width * j + 4 * i + 1];
			tmp[2] = out[4 * width * j + 4 * i + 2];
			tmp[3] = out[4 * width * j + 4 * i + 3];

			out[4 * width * j + 4 * i + 0] = out[4 * width * t + 4 * i + 0];
			out[4 * width * j + 4 * i + 1] = out[4 * width * t + 4 * i + 1];
			out[4 * width * j + 4 * i + 2] = out[4 * width * t + 4 * i + 2];
			out[4 * width * j + 4 * i + 3] = out[4 * width * t + 4 * i + 3];

			out[4 * width * t + 4 * i + 0] = tmp[0];
			out[4 * width * t + 4 * i + 1] = tmp[1];
			out[4 * width * t + 4 * i + 2] = tmp[2];
			out[4 * width * t + 4 * i + 3] = tmp[3];
		}
	}

	unsigned int errorCode = lodepng::encode(filePath.data(), out, width, height);
	if ( errorCode != 0 )
		Throw(std::string("Encode PNG failed with error: ") + lodepng_error_text(errorCode) + "\n File : " + filePath.data());
}
//-----------------------------------------------------------------------------