#pragma once

class Sound
{
	friend class AudioEngine;
public:
	void Play(int loops = 0);
	void Play(int loops, Uint8 distance);

	const std::string& GetFilePath() const
	{
		return m_filePath;
	}

private:
	static int m_lastChannel;
	Mix_Chunk *m_chunk = nullptr;
	std::string m_filePath;
};

class Music
{
	friend class AudioEngine;
public:
	void Play(int loops = -1);

	static int Playing();

	static void Pause();

	static void Resume();

	static void Stop();

	const std::string& GetFilePath() const
	{
		return m_filePath;
	}

private:
	Mix_Music *m_music = nullptr;
	std::string m_filePath;
};

class AudioEngine
{
public:
	bool Init();
	void Destroy();

	Sound LoadSound(std::string_view filePath);
	Music LoadMusic(std::string_view filePath);

private:
	bool m_initialized;
	std::unordered_map<std::string, Mix_Chunk*> m_effectMap;
	std::unordered_map<std::string, Mix_Music*> m_musicMap;
};