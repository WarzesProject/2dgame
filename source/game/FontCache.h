#pragma once

#include "SpriteFont.h"

class FontCache
{
public:
	void destroy();
	SpriteFont getFont(const std::string fontPath, int size);

private:
	std::map<std::string, SpriteFont> m_fontMap;
};