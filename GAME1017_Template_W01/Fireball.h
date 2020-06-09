#pragma once
#include <vector>
#include "Sprite.h"

class Fireball : public Sprite
{
private:
	int m_speed;
	int face;
public:
	Fireball(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int spd, int face);
	void Update();
};