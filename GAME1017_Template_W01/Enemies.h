#pragma once
#include <string>

#include "Sprite.h"
#include "Entity.h"
#include "PlatformPlayer.h"

enum Status { IDLE, PATROLING, SEEKING, HIDING, FLEEING, ATTACKING, STUNNED, DYING, DEAD };

const float HIDECOOLDOWN = 10.0;
const float HIDETIME = 4.0;

class Enemies :public Entity
{
public:
	Enemies(SDL_Rect s, SDL_FRect d, SDL_FRect b, SDL_Renderer* r, SDL_Texture* t, Animator* animator);
	
	virtual void Update() = 0;
	virtual void Render() = 0;

	void Flee(float RUNSPEED, float squareDistToPlayer, float ATTACKDISTANCE, float STOPDISTANCE);
	void Seek(float RUNSPEED, float squareDistToPlayer, float STOPDISTANCE, float ATTACKDISTANCE, bool knowWherePlayer);
	void Hide(float RUNSPEED, float squareDistToPlayer, float ATTACKDISTANCE, float STOPDISTANCE, const float FLEEPROCENTAGE);
	
	bool getActive();
	void setActive(bool a);
	bool getAlive();
	void setAlive(bool a);
	void getDamage(int dmg);
	
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
	
	Uint32 attackWaitTime;
	Uint32 lastAttackTime;
	
	Uint32 m_lastDetectTime;
	Uint32 m_hided;
	Uint32 m_waitBehindTheCover;
	Uint32 m_noLOStime;
	Uint32 m_hidingTime;

	int m_dyingAnimation;
	
	std::string enemyType;
	Status curStatus;
	bool m_playerLOS;
};