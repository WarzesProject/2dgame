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
void ResourceManager::Destroy()
{
	m_fontCache.Destroy();
}
//-----------------------------------------------------------------------------
Sound ResourceManager::GetSound(const std::string soundPath)
{
	return m_audioEngine->LoadSound(soundPath);
}
//-----------------------------------------------------------------------------
Music ResourceManager::GetMusic(const std::string musicPath)
{
	return m_audioEngine->LoadMusic(musicPath);
}
//-----------------------------------------------------------------------------
Texture ResourceManager::GetTexture(const std::string texturePath)
{
	return m_textureCache.GetTexture(texturePath);
}
//-----------------------------------------------------------------------------
SpriteFont ResourceManager::GetFont(const std::string fontPath, int size/* = 32*/)
{
	return m_fontCache.GetFont(fontPath, size);
}
//-----------------------------------------------------------------------------