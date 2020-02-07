#include "stdafx.h"
#include "FontCache.h"
//-----------------------------------------------------------------------------
SpriteFont FontCache::GetFont(const std::string fontPath, int size)
{
	auto mit = m_fontMap.find(fontPath);

	if ( mit == m_fontMap.end() )
	{
		SDL_Log("Font %s Load", fontPath.data());

		SpriteFont font;
		font.Init(fontPath, size);
		m_fontMap.insert(make_pair(fontPath + std::to_string(size), font));
		return font;
	}

	return mit->second;
}
//-----------------------------------------------------------------------------
void FontCache::Destroy()
{
	for ( auto &p : m_fontMap )
		p.second.Dispose();
}
//-----------------------------------------------------------------------------