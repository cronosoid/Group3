#pragma once
#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_

#include "Sprite.h"
#include "Entity.h"

const int ATTACKINTERVAL = 40; // In frames
const int STOPAFTERGOTATTACKED = 8; // In frames

const int MELEESTUNTIME = 24; // In frames
const int FIREBALLSTUNTIME = 24; // In frames

const double MELEECOOLDOWN = 0.8;
const double MAGICCOOLDOWN = 0.8;
const double FIREBALLCOST = 10.0;

const int CANNOTCONTROLTIME = 20;

class PlatformPlayer : public Entity
{
public: // Variables
	const double m_maxSoul = 500.0;
	const int m_meeleDmg = 50;
	const int m_magicDmg = 100;
	const int m_soulRecover = 10;
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
	void SetLastAttackedTime() { m_lastAttacked = ATTACKINTERVAL; }

private:
	double m_curSoul;
	Uint32 lastMeleeTime;
	Uint32 lastMagicTime;

	
};

#endif