#pragma once
#include <string>
#include <iostream>
#include "Sprite.h"
#include "MapObject.h"

#define GRAV 3.0
#define MAXFLYPOWER 80.0
#define JUMPFORCE 35.0
#define FALLCOF 20.0

class Animator;

class Entity :public Sprite
{
public:
	Entity(SDL_Rect s, SDL_FRect d, SDL_FRect b, SDL_Renderer* r, SDL_Texture* t, Animator* animator, bool player = false);
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	bool IsGrounded();
	void SetGrounded(bool g, MapObject* Floor = nullptr);
	double GetVelX();
	double GetVelY();
	void SetX(float y);
	void SetY(float y);
	void movementUpdate();
	Animator* getAnimator() { return animator; }
	void addAnimator(Animator* animator);
	SDL_FRect* getGlobalDst() { return &globalDst; }
	MapObject* GetFloor() { return m_floor; }
	void Stun(int frames) { m_stunTime = abs(frames); }
	
	SDL_FRect* GetBody() { return &m_body; }

	bool IsPlayer() { return m_player; }
protected:
	bool m_player;
	bool m_grounded;
	bool m_gravityEnable;
	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxJumpVelocity,
		m_maxFallVelocity,
		m_drag,
		m_grav,
		m_speed;
	Uint32 flyingTime;
	Uint32 m_stunTime;
	
	MapObject* m_floor;
	Animator* animator;
	SDL_FRect globalDst;
	SDL_FRect m_body;
};

#include "Animator.h"