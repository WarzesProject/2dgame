#include "stdafx.h"
#include "AudioEngine.h"

//-----------------------------------------------------------------------------
constexpr auto ALLOCATE_CHANNELS = 16;
//-----------------------------------------------------------------------------
int Sound::m_lastChannel = 0;
//-----------------------------------------------------------------------------
void Sound::Play(int loops)
{
	if ( Mix_PlayChannel(-1, m_chunk, loops) == -1 )
	{
		if ( Mix_PlayChannel(0, m_chunk, loops) == -1 )
			Throw("Mix_PlayChannel : " + std::string(Mix_GetError()));
	}
}
//-----------------------------------------------------------------------------
void Sound::Play(int loops, Uint8 distance)
{
	if ( Mix_SetDistance(m_lastChannel, distance) )
	{
		if ( (m_lastChannel = Mix_PlayChannel(m_lastChannel, m_chunk, loops)) == -1 )
		{
			if ( (m_lastChannel = Mix_PlayChannel(0, m_chunk, loops)) == -1 )
				Throw("Mix_PlayChannel : " + std::string(Mix_GetError()));
		}
	}
	else
	{
		Mix_SetDistance(MIX_CHANNEL_POST, distance);

		if ( (m_lastChannel = Mix_PlayChannel(-1, m_chunk, loops)) == -1 )
		{
			if ( (m_lastChannel = Mix_PlayChannel(0, m_chunk, loops)) == -1 )
				Throw("Mix_PlayChannel : " + std::string(Mix_GetError()));
		}
	}

	m_lastChannel++;
	if ( m_lastChannel >= Mix_AllocateChannels(-1) )
		m_lastChannel = 0;
}
//-----------------------------------------------------------------------------
void Music::Play(int loops)
{
	if ( Mix_PlayMusic(m_music, loops) == -1 )
		Throw("Mix_PlayMusic : " + std::string(Mix_GetError()));
}
//-----------------------------------------------------------------------------
int Music::Playing()
{
	return Mix_PlayingMusic();
}
//-----------------------------------------------------------------------------
void Music::Pause()
{
	Mix_PauseMusic();
}
//-----------------------------------------------------------------------------
void Music::Resume()
{
	Mix_ResumeMusic();
}
//-----------------------------------------------------------------------------
void Music::Stop()
{
	Mix_HaltMusic();
}
//-----------------------------------------------------------------------------
bool AudioEngine::Init()
{
	if ( m_initialized )
	{
		SDL_Log("Tried to initialize Audio engine twice");
		return true;
	}

	if ( Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 1 )
		Throw("Mix_Init : " + std::string(Mix_GetError()));

	if ( Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) )
		Throw("Mix_OpenAudio : " + std::string(Mix_GetError()));

	Mix_AllocateChannels(ALLOCATE_CHANNELS);

	return m_initialized = true;
}
//-----------------------------------------------------------------------------
void AudioEngine::Destroy()
{
	if ( m_initialized )
	{
		for ( auto& it : m_effectMap )
		{
			Mix_FreeChunk(it.second);
		}

		for ( auto& it : m_musicMap )
		{
			Mix_FreeMusic(it.second);
		}

		m_initialized = false;
		m_effectMap.clear();
		m_musicMap.clear();
		Mix_Quit();
	}
}
//-----------------------------------------------------------------------------
Sound AudioEngine::LoadSound(std::string_view filePath)
{
	Sound effect;

	auto it = m_effectMap.find(filePath.data());

	if ( it == m_effectMap.end() )
	{
		Mix_Chunk* chunk = Mix_LoadWAV(filePath.data());

		if ( chunk == nullptr )
			Throw("Mix_LoadWAV : " + std::string(Mix_GetError()));
		else
		{
			effect.m_chunk = chunk;
			effect.m_filePath = filePath;
			m_effectMap[filePath.data()] = chunk;
		}
	}
	else
	{
		effect.m_chunk = it->second;
		effect.m_filePath = filePath;
	}

	return effect;
}
//-----------------------------------------------------------------------------
Music AudioEngine::LoadMusic(std::string_view filePath)
{
	Music music;

	auto it = m_musicMap.find(filePath.data());

	if ( it == m_musicMap.end() )
	{
		Mix_Music* mixMusic = Mix_LoadMUS(filePath.data());

		if ( mixMusic == nullptr )
			Throw("Mix_LoadMUS : " + std::string(Mix_GetError()));
		else
		{
			music.m_music = mixMusic;
			music.m_filePath = filePath;
			m_musicMap[filePath.data()] = mixMusic;
		}
	}
	else
	{
		music.m_music = it->second;
	}

	return music;
}
//-----------------------------------------------------------------------------