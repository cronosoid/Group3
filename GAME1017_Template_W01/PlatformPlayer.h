#pragma once
#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_

#include "Sprite.h"
#include "Entity.h"

const int ATTACKINTERVAL = 40; // In frames
const int STOPAFTERGOTATTACKED = 8; // In frames
const double MELEECOOLDOWN = 1.0;
const double MAGICCOOLDOWN = 1.0;
const double FIREBALLCOST = 10.0;

class PlatformPlayer : public Entity
{
public: // Variables
	const double m_maxSoul = 100;
	const int m_meeleDmg = 20;
	const int m_magicDmg = 35;
	const int m_soulRecover = 5;
	int movement[2] = { 0,0 };
public: // Methods
	PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator = nullptr);
	~PlatformPlayer();

	void Update();
	void Render();
	
	double GetSoul();
	void SoulRecover();
	void ChangeSoul(int amnt);
	void Meele();
	//void Fireball();
	double getHealPercent();
	Uint32 getMeleeTime() { return lastMeleeTime; }
	void setMeleeTime() { lastMeleeTime = SDL_GetTicks(); }
	Uint32 getMagicTime() { return lastMagicTime; }
	void setMagicTime() { lastMagicTime = SDL_GetTicks(); }
	void getDamage(int dmg);
	Uint32 GetLastAttackedTime() { return m_lastAttacked; }
	void SetLastAttackedTime() { m_lastAttacked = ATTACKINTERVAL; }

private:
	double m_curSoul;
	Uint32 lastMeleeTime;
	Uint32 lastMagicTime;
	Uint32 m_lastAttacked = 0;
};

#endif