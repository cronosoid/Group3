#pragma once
#include <string>
#include <iostream>
#include "Sprite.h"

#define GRAV 3.0
#define JUMPFORCE 40.0

class Entity :public Sprite
{
public:
	Entity(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	bool IsGrounded();
	void SetGrounded(bool g, SDL_FRect* Floor = nullptr);
	double GetVelX();
	double GetVelY();
	void SetX(float y);
	void SetY(float y);

protected:
	bool m_grounded;
	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_grav;
	SDL_FRect* Floor;
};