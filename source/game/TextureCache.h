#pragma once

#include "Texture.h"

class TextureCache
{
public:
	Texture GetTexture(std::string_view texturePath);

private:
	std::unordered_map<std::string, Texture> m_textureMap;
};