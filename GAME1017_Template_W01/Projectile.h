#pragma once
#include <vector>
#include "Sprite.h"

class Projectile : public Sprite
{
private:
	double m_speed;
	int face;
	double m_damage;
public:
	Projectile(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double spd, int face, double damage = 0);
	void Update();
};