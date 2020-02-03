#pragma once

#include "TextureCache.h"
#include "AudioEngine.h"
#include "FontCache.h"

class ResourceManager
{
public:
	static bool Init(AudioEngine *audioEngine);
	static void destroy();

	static Sound getSound(const std::string soundPath);
	static Music getMusic(const std::string musicPath);

	static Texture getTexture(const std::string texturePath);

	static SpriteFont getFont(const std::string fontPath, int size = 32);

private:
	static TextureCache m_textureCache;
	static AudioEngine *m_audioEngine;
	static FontCache m_fontCache;
};