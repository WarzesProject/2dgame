#include "stdafx.h"
#include "ResourceManager.h"
//-----------------------------------------------------------------------------
TextureCache ResourceManager::m_textureCache;
AudioEngine *ResourceManager::m_audioEngine = nullptr;
FontCache ResourceManager::m_fontCache;
//-----------------------------------------------------------------------------
bool ResourceManager::Init(AudioEngine *audioEngine)
{
	m_audioEngine = audioEngine;
	return true;
}
//-----------------------------------------------------------------------------
void ResourceManager::destroy()
{
	m_fontCache.destroy();
}
//-----------------------------------------------------------------------------
Sound ResourceManager::getSound(const std::string soundPath)
{
	return m_audioEngine->LoadSound(soundPath);
}
//-----------------------------------------------------------------------------
Music ResourceManager::getMusic(const std::string musicPath)
{
	return m_audioEngine->LoadMusic(musicPath);
}
//-----------------------------------------------------------------------------
Texture ResourceManager::getTexture(const std::string texturePath)
{
	return m_textureCache.GetTexture(texturePath);
}
//-----------------------------------------------------------------------------
SpriteFont ResourceManager::getFont(const std::string fontPath, int size/* = 32*/)
{
	return m_fontCache.getFont(fontPath, size);
}
//-----------------------------------------------------------------------------