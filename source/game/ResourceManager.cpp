#include "stdafx.h"
#include "ResourceManager.h"
//-----------------------------------------------------------------------------
TODO("сделать очистку ресурсов по завершению");
//-----------------------------------------------------------------------------
TextureCache ResourceManager::m_textureCache;
std::shared_ptr<AudioSystem> ResourceManager::m_audioSystem = nullptr;
FontCache ResourceManager::m_fontCache;
ApplicationConfig::Resources *ResourceManager::m_config = nullptr;
//-----------------------------------------------------------------------------
ResourceManager::ResourceManager(ApplicationConfig::Resources &config, std::shared_ptr<AudioSystem> audioSystem)
{
	m_config = &config;
	m_audioSystem = audioSystem;
}
//-----------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	m_fontCache.Destroy();
	m_audioSystem.reset();
}
//-----------------------------------------------------------------------------
Sound ResourceManager::GetSound(const std::string_view soundPath)
{
	const std::string path = m_config->coreFolder + std::string(soundPath);
	return m_audioSystem->LoadSound(path);
}
//-----------------------------------------------------------------------------
Music ResourceManager::GetMusic(const std::string_view musicPath)
{
	const std::string path = m_config->coreFolder + std::string(musicPath);
	return m_audioSystem->LoadMusic(path);
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