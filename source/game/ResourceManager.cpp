#include "stdafx.h"
#include "ResourceManager.h"
//-----------------------------------------------------------------------------
TODO("сделать очистку ресурсов по завершению");
//-----------------------------------------------------------------------------
TextureCache ResourceManager::m_textureCache;
std::shared_ptr<AudioEngine> ResourceManager::m_audioEngine = nullptr;
FontCache ResourceManager::m_fontCache;
ApplicationConfig::Resources *ResourceManager::m_config = nullptr;
//-----------------------------------------------------------------------------
ResourceManager::ResourceManager(ApplicationConfig::Resources &config, std::shared_ptr<AudioEngine> audioEngine)
{
	m_config = &config;
	m_audioEngine = audioEngine;
}
//-----------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	m_fontCache.Destroy();
	m_audioEngine.reset();
}
//-----------------------------------------------------------------------------
Sound ResourceManager::GetSound(const std::string_view soundPath)
{
	const std::string path = m_config->coreFolder + std::string(soundPath);
	return m_audioEngine->LoadSound(path);
}
//-----------------------------------------------------------------------------
Music ResourceManager::GetMusic(const std::string_view musicPath)
{
	const std::string path = m_config->coreFolder + std::string(musicPath);
	return m_audioEngine->LoadMusic(path);
}
//-----------------------------------------------------------------------------
Texture ResourceManager::GetTexture(const std::string_view texturePath)
{
	const std::string path = m_config->coreFolder + std::string(texturePath);
	return m_textureCache.GetTexture(path);
}
//-----------------------------------------------------------------------------
SpriteFont ResourceManager::GetFont(const std::string_view fontPath, int size)
{
	const std::string path = m_config->coreFolder + std::string(fontPath);
	return m_fontCache.GetFont(path, size);
}
//-----------------------------------------------------------------------------