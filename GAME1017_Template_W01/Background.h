#pragma once
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "Sprite.h"
#include <SDL.h>

class Background : public Sprite
{
private:

public:
	Background(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);

};

#endif

