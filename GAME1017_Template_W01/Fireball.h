#pragma once
#include <vector>
#include "Sprite.h"

class Fireball : public Sprite
{
private:
	int m_speed;

public:
	Fireball(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int spd);
	void Update();
};
