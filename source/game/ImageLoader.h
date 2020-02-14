#pragma once

#include "Texture.h"

class ImageLoader
{
public:
	static Texture Create(int width, int height);
	static Texture Load(std::string_view filePath);
};