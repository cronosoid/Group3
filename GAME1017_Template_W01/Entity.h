#pragma once
#include <string>
#include <iostream>
#include "Sprite.h"

#define GRAV 3.0
#define JUMPFORCE 40.0
#define FALLCOF 20.0

class Animator;

class Entity :public Sprite
{
public:
	Entity(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator, bool player = false);
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
	void movementUpdate();
	Animator* getAnimator() { return animator; }
	void addAnimator(Animator* animator);
	SDL_FRect* getGlobalDst() { return &globalDst; }
protected:
	bool m_player;
	bool m_grounded;
	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxJumpVelocity,
		m_maxFallVelocity,
		m_drag,
		m_grav;
	Uint32 flyingTime;
	SDL_FRect* Floor;
	Animator* animator;
	SDL_FRect globalDst;
};

#include "Animator.h"