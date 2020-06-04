#include "TextureManager.h"
#include "Engine.h"
#include <iostream>

void TextureManager::Init()
{
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
		std::cout << "Image init success!" << std::endl;
	else
		std::cout << "Image init failed: ERROR - " << IMG_GetError() << std::endl;
}

void TextureManager::RegisterTexture(const char * path, const std::string key)
{
	SDL_Texture* temp = IMG_LoadTexture(Engine::Instance().GetRenderer(), path);
	if (temp == nullptr)
		std::cout << "Could not load texture: Error - " << IMG_GetError() << std::endl;
	else
		s_textures.emplace(key, temp);
}

SDL_Texture* TextureManager::GetTexture(const std::string key) { return s_textures[key]; }

void TextureManager::Quit()
{
	for (auto const& i : s_textures)
	{
		SDL_DestroyTexture(s_textures[i.first]);
	}
	s_textures.clear();
}

std::map<std::string, SDL_Texture*> TextureManager::s_textures;
