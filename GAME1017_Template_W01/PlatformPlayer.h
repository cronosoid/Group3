#pragma once
#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_
#define GRAV 6.0
#define JUMPFORCE 60.0

#include "Sprite.h"

class PlatformPlayer : public Sprite
{
public:
	PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	const double m_maxSoul = 100;
	int m_meeleDmg,
		m_fireDmg,
		m_soulRcvrd;
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
	void SoulRcvry();
	SDL_FRect* Meele();
	void Fireball();
		 

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
};

#endif