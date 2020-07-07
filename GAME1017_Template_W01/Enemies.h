#pragma once
#include <string>

#include "Sprite.h"
#include "Entity.h"

enum Status { IDLE, PATROLING, SEEKING, FLEEING, ATTACKING, DEAD };

class Enemies :public Entity
{
public:
	Enemies(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator);
	virtual void Update() = 0;
	virtual void Render() = 0;
	bool getActive();
	void setActive(bool a);
	bool getAlive();
	void setAlive(bool a);
	void getDamage(int dmg);
	int getDamageNum() { return damage;}
	SDL_Texture* getTex();
	std::string getType();

protected:
	bool isActive,
		 isAlive;
	 int  damage,
		 defence,
		 health;
	Uint32 lastAttackTime;
	std::string enemyType;
	Status curStatus;
};