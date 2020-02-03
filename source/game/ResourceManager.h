#pragma once

#include "TextureCache.h"
#include "AudioEngine.h"
#include "FontCache.h"

class ResourceManager
{
public:
	static bool Init(AudioEngine *audioEngine);
	static void Destroy();

	static Sound GetSound(const std::string soundPath);
	static Music GetMusic(const std::string musicPath);

	static Texture GetTexture(const std::string texturePath);

	static SpriteFont GetFont(const std::string fontPath, int size = 32);

private:
	static TextureCache m_textureCache;
	static AudioEngine *m_audioEngine;
	static FontCache m_fontCache;
};