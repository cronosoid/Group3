#pragma once
#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_

#include "Sprite.h"
#include "Fireball.h"
#include "Entity.h"

const int MELEECOOLDOWN = 1.0;

class PlatformPlayer : public Entity
{
public: // Variables
	const double m_maxSoul = 100;
	const int m_meeleDmg = 20;
	const int m_fireDmg = 0;
	const int m_soulRecover = 0;
	int movement[2] = { 0,0 };
public: // Methods
	PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	void Update();
	void Render();
	
	double GetSoul();
	void SoulRcvry();
	void Meele();
	void Fireball();
	double getHeaPercent();
	Uint32 getMeleeTime() { return lastMeleeTime; }
	void setMeleeTime() { lastMeleeTime = SDL_GetTicks(); }

private:
	double m_curSoul;
	Uint32 lastMeleeTime;
	
};

#endif