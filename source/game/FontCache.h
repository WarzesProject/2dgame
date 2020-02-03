#pragma once

#include "SpriteFont.h"

class FontCache
{
public:
	void Destroy();
	SpriteFont GetFont(const std::string fontPath, int size);

private:
	std::map<std::string, SpriteFont> m_fontMap;
};