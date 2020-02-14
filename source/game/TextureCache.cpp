#include "stdafx.h"
#include "TextureCache.h"
#include "ImageLoader.h"
//-----------------------------------------------------------------------------
Texture TextureCache::GetTexture(std::string_view texturePath)
{
	auto mit = m_textureMap.find(texturePath.data());
	if ( mit == m_textureMap.end() )
	{
		SDL_Log("Texture %s Load", texturePath.data());

		Texture newTexture = ImageLoader::Load(texturePath);

		if ( newTexture.height > 0 && newTexture.width > 0 )
			m_textureMap.insert(make_pair(texturePath, newTexture));
		return newTexture;
	}
	return mit->second;
}
//-----------------------------------------------------------------------------