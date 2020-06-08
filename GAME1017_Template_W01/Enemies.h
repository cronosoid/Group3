#pragma once
#include <string>

#include "Sprite.h"

class Enemies:public Sprite
{
public:
	Enemies(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	virtual void Update()=0;
	virtual void Render()=0;
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
	bool getActive();
	void setActive(bool a);
	bool getAlive();
	void setAlive(bool a);
	void decHealth(int attack);
	SDL_Texture* getTex();
	std::string getType();
	
protected:
	bool m_grounded,
		isActive,
	isAlive;
	double m_accelX,
		m_accelY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_grav;
	int AttackCD,
		Attack,
		Defence,
		Health;
	std::string EnemyType;
};
