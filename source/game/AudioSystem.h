﻿#pragma once

#include "ApplicationConfig.h"

class Sound
{
	friend class AudioSystem;
public:
	// если loops == -1, то проигрывать бесконечно
	void Play(int loops = 0);
	void Play(int loops, uint8_t distance);

private:
	static int m_lastChannel;
	Mix_Chunk *m_chunk = nullptr;
	std::string m_filePath;
};

class Music
{
	friend class AudioSystem;
public:
	// если loops == -1, то проигрывать бесконечно
	void Play(int loops = -1);
	void SetVolume(int volume = 128);
	static int Playing();
	static void Pause();
	static void Resume();
	static void Stop();	

private:
	Mix_Music *m_music = nullptr;
	std::string m_filePath;
};

class AudioSystem
{
public:
	AudioSystem(ApplicationConfig::Audio &config);
	~AudioSystem();

	Sound LoadSound(std::string_view filePath);
	Music LoadMusic(std::string_view filePath);

private:
	AudioSystem() = delete;
	AudioSystem(const AudioSystem&) = delete;
	AudioSystem(AudioSystem&&) = delete;
	AudioSystem& operator=(const AudioSystem&) = delete;
	AudioSystem& operator=(AudioSystem&&) = delete;

	ApplicationConfig::Audio &m_config;
	std::unordered_map<std::string, Mix_Chunk*> m_soundMap;
	std::unordered_map<std::string, Mix_Music*> m_musicMap;
};