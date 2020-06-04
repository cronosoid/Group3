#include "SoundManager.h"
#include <algorithm>
#include <iostream>

void SoundManager::Init()
{
	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048) == 0)
	{
		std::cout << "Mix init success!" << std::endl;
		PanReset();
	}
	else 
		std::cout << "Mix init failed: ERROR - " << Mix_GetError() << std::endl;
}

void SoundManager::AllocateChannels(const int channels)
{
	Mix_AllocateChannels(channels);
}

bool SoundManager::Load(const std::string& file_name, const std::string& id, const SoundType type)
{
	if (type == SOUND_MUSIC)
	{
		Mix_Music* pMusic = Mix_LoadMUS(file_name.c_str());
		if (pMusic == nullptr)
		{
			std::cout << "Could not load music: ERROR - " << Mix_GetError() << std::endl;
			return false;
		}
		s_music[id] = pMusic;
		return true;
	}
	else if (type == SOUND_SFX)
	{
		Mix_Chunk* pChunk = Mix_LoadWAV(file_name.c_str());
		if (pChunk == nullptr)
		{
			std::cout << "Could not load SFX: ERROR - " << Mix_GetError() << std::endl;
			return false;
		}
		s_sfxs[id] = pChunk;
		return true;
	}
	return false;
}

void SoundManager::Unload(const std::string& id, const SoundType type)
{
	if (type == SOUND_MUSIC && s_music.find(id) != s_music.end())
	{
		Mix_FreeMusic(s_music[id]);
		s_music.erase(id);
	}
	else if (type == SOUND_SFX && s_sfxs.find(id) != s_sfxs.end())
	{
		Mix_FreeChunk(s_sfxs[id]);
		s_sfxs.erase(id);
	}
	else
	{
		std::cout << "Could not unload '" << id << "' - id not found!" << std::endl;
	}	
}

void SoundManager::PlayMusic(const std::string& id, const int loop/* = -1 */, const int fade_in/* = 0 */)
{	
	std::cout << "Playing music..." << fade_in << std::endl;
	if (Mix_FadeInMusic(s_music[id], loop, fade_in) == -1)
	{
		std::cout << "Unable to play music: ERROR - " << Mix_GetError() << std::endl;
	}
}

void SoundManager::StopMusic(const int fade_out/* = 0 */)
{
	if (Mix_PlayingMusic())
	{
		Mix_FadeOutMusic(fade_out);
	}	
}

void SoundManager::PauseMusic()
{
	if (Mix_PlayingMusic())
	{
		Mix_PauseMusic();
	}	
}

void SoundManager::ResumeMusic()
{
	if (Mix_PausedMusic())
	{
		Mix_ResumeMusic();
	}	
}

void SoundManager::PlaySound(const std::string& id, const int loop/* = 0 */, const int channel/* = -1 */)
{
	if (Mix_PlayChannel(channel, s_sfxs[id], loop) == -1)
	{
		std::cout << "Unable to play SFX: ERROR - " << Mix_GetError() << std::endl;
	}	
}

void SoundManager::SetMusicVolume(const int vol)
{
	if (vol >= 0 && vol <= 128)
	{
		Mix_VolumeMusic(vol);
	}
}

void SoundManager::SetSoundVolume(const int vol)
{
	if (vol >= 0 && vol <= 128)
	{
		Mix_Volume(-1, vol);
	}	
}

void SoundManager::SetAllVolume(const int vol)
{
	SetMusicVolume(vol);
	SetSoundVolume(vol);
}

void SoundManager::PanLeft(const unsigned increment, const int channel)
{
	PanSet(s_pan - increment, channel);
}

void SoundManager::PanRight(const unsigned increment, const int channel)
{
	PanSet(s_pan + increment, channel);
}

void SoundManager::PanReset(const int channel)
{
	PanSet(50, channel);
}

void SoundManager::PanSet(const int amount, const int channel)
{
	s_pan = std::max(0,std::min(amount,100)); // Old clamp.
	const int leftVol = (int)(255 * std::min((s_pan - 100.0) / (50.0 - 100.0), 1.0)); // z-min/max-min
	const int rightVol = (int)(255 * std::min(s_pan / 50.0, 1.0));
	Mix_SetPanning(channel, leftVol, rightVol);
}

void SoundManager::Quit()
{
	// Clean up sound effects.
	if (Mix_Playing(-1))
	{
		Mix_HaltChannel(-1); // Halt all channels.
	}
		
	for (auto const& i : s_sfxs)
	{
		Mix_FreeChunk(s_sfxs[i.first]);
	}
	s_sfxs.clear();
	
	// Clean up music.
	if (Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
		
	for (auto const& i : s_music)
	{
		Mix_FreeMusic(s_music[i.first]);
	}	
	s_music.clear();
	
	// Quit.
	Mix_CloseAudio();
	Mix_Quit();
}

std::map<std::string, Mix_Chunk*> SoundManager::s_sfxs;
std::map<std::string, Mix_Music*> SoundManager::s_music;
int SoundManager::s_pan;