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
	
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	bool IsGrounded();
	void SetGrounded(bool g);
	double GetVelX();
	double GetVelY();
	void SetX(float y);
	void SetY(float y);
	double GetSoul();

private:
	bool m_grounded;

	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_grav,
		m_curSoul;

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