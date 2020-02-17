#include "stdafx.h"
#include "TextureCache.h"
//-----------------------------------------------------------------------------
Texture TextureCache::GetTexture(std::string_view texturePath)
{
	auto mit = m_textureMap.find(texturePath.data());
	if ( mit == m_textureMap.end() )
	{
		SDL_Log("Texture %s Load", texturePath.data());

		Texture newTexture(texturePath);
		if ( newTexture.IsValid() )
			m_textureMap.insert(make_pair(texturePath, newTexture));
		return newTexture;
	}
	return mit->second;
}
//-----------------------------------------------------------------------------