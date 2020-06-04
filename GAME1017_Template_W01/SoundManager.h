#pragma once
#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include <iostream>
#include <string>
#include <map> 
#include <SDL_mixer.h>

enum SoundType
{
	SOUND_MUSIC = 0,
	SOUND_SFX = 1
};

class SoundManager
{
public:
	static void Init();
	static void AllocateChannels(const int channels);
	static bool Load(const std::string& file_name, const std::string& id, SoundType type);
	static void Unload(const std::string& id, SoundType type);
	static void PlayMusic(const std::string& id, int loop = -1, int fade_in = 0);
	static void StopMusic(int fade_out = 0);
	static void PauseMusic();
	static void ResumeMusic();
	static void PlaySound(const std::string& id, int loop = 0, int channel = -1);
	static void SetMusicVolume(const int vol);
	static void SetSoundVolume(const int vol);
	static void SetAllVolume(const int vol);
	
	// Panning converts a single slider value from 0 to 100 to the stereo speaker volumes.
	static void PanLeft(const unsigned increment = 5, const int channel = MIX_CHANNEL_POST);
	static void PanRight(const unsigned increment = 5, const int channel = MIX_CHANNEL_POST);
	static void PanReset(const int channel = MIX_CHANNEL_POST);
	static void PanSet(const int amount, const int channel = MIX_CHANNEL_POST);
	static void Quit();
	
private: // Methods.
	SoundManager();
	
private: // Properties.
	static std::map<std::string, Mix_Chunk*> s_sfxs;
	static std::map<std::string, Mix_Music*> s_music;
	static int s_pan; // A slider value from 0 to 100. 0 = full left, 100 = full right.
};

typedef SoundManager SOMA;

#endif 