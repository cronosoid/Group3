#pragma once
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include <map>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class TextureManager
{
public:
	static void Init();
	static void RegisterTexture(const char* path, const std::string key);
	static SDL_Texture* GetTexture(const std::string key);
	static void Quit();
private:
	static std::map<std::string, SDL_Texture*> s_textures;
private:
	TextureManager();
};

typedef TextureManager TEMA;

#endif