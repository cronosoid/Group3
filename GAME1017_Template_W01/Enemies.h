#pragma once
#include <string>

#include "Sprite.h"
#include "Entity.h"

enum Status { IDLE, PATROLING, SEEKING, FLEEING, ATTACKING, STUNNED, DEAD };

class Enemies :public Entity
{
public:
	Enemies(SDL_Rect s, SDL_FRect d, SDL_FRect b, SDL_Renderer* r, SDL_Texture* t, Animator* animator);
	virtual void Update() = 0;
	virtual void Render() = 0;
	bool getActive();
	void setActive(bool a);
	bool getAlive();
	void setAlive(bool a);
	void getDamage(int dmg);
	void Stun(int frames) { m_stunTime = abs(frames); }
	int getDamageNum() { return damage;}
	SDL_Texture* getTex();
	std::string getType();
	int GetHealth() { return health; }
	int GetMaxHealth() { return maxHealth; }
	bool HavePlayerLOS() { return m_playerLOS; }
	
protected:
	bool isActive,
		 isAlive;
	int  damage,
		defence,
		health,
		maxHealth;
	Uint32 lastAttackTime;
	Uint32 m_stunTime;
	Uint32 m_lastDetectTime;
	std::string enemyType;
	Status curStatus;
	bool m_playerLOS;
};