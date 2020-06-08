#include "SoulBar.h"

SoulBar::SoulBar(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t):
Sprite(s, d, r, t) {}

double SoulBar::soulChange(double maxSoul, double damageSoul)
{
	m_pSoulLeft = (maxSoul - damageSoul) / maxSoul;

	return m_pSoulLeft;
}
