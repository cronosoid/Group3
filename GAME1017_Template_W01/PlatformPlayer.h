#pragma once
#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_

#include "Sprite.h"
#include "Entity.h"

class PlatformPlayer : public Entity
{
public: // Variables
	const double m_maxSoul = 100;
	const int m_meeleDmg = 0;
	const int m_fireDmg = 0;
	const int m_soulRecover = 0;
	int movement[2] = { 0,0 };
public: // Methods
	PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	void Update();
	void Render();
	double GetSoul();
	void SoulRcvry();
	SDL_FRect* Meele();
	void Fireball();

private:
	double m_curSoul;
};

#endif