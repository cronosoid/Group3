#pragma once
#include "Sprite.h"

class SoulBar : public Sprite
{
public:
	SoulBar(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	double getSoul() { return m_pSoulLeft; }
	double soulChange(double maxSoul, double damageSoul);

private:
	double m_pSoulLeft{};
};
