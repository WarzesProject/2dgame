#pragma once

#include "TextureCache.h"
#include "AudioSystem.h"
#include "FontCache.h"
#include "ApplicationConfig.h"

class ResourceManager
{
public:
	ResourceManager(ApplicationConfig::Resources &config, std::shared_ptr<AudioSystem> audioEngine);
	~ResourceManager();

	static Sound GetSound(const std::string_view soundPath);
	static Music GetMusic(const std::string_view musicPath);
	static Texture GetTexture(const std::string_view texturePath);
	static SpriteFont GetFont(const std::string_view fontPath, int size = 32);

private:
	static TextureCache m_textureCache;
	static std::shared_ptr<AudioSystem> m_audioEngine;
	static FontCache m_fontCache;
	static ApplicationConfig::Resources *m_config;
};