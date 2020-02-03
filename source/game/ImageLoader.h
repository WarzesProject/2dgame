#pragma once

#include "Texture.h"

class ImageLoader
{
public:
	static Texture Create(GLsizei width, GLsizei height);

	static Texture Load(std::string_view filePath);

	static void SavePNG(GLint x, GLint y, GLsizei width, GLsizei height, std::string_view filePath);
};