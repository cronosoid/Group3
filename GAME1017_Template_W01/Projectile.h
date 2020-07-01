#pragma once
#include <vector>
#include "Sprite.h"

class Projectile : public Sprite
{
private:
	double m_speed;
	int face;
	double m_damage;

	int m_curFrame = 0;
	int m_curAnimFrame = 0;
	int m_framesFrequency;
	int m_maxFrames;
	int m_moveX;
	int m_moveY;
public:
	Projectile(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double spd, int face, double damage = 0,
		int framesFrequency = 0, int maxFrames = 0, int moveX = 0, int m_moveY = 0);
	void Update();
};