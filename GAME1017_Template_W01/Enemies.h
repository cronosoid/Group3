#pragma once
#include <string>

#include "Sprite.h"
#include "Entity.h"

class Enemies :public Entity
{
public:
	Enemies(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	virtual void Update() = 0;
	virtual void Render() = 0;
	bool getActive();
	void setActive(bool a);
	bool getAlive();
	void setAlive(bool a);

  void damage(int dmg);
	SDL_Texture* getTex();
	std::string getType();

protected:
	bool isActive,
		isAlive;
	int attackCD,
		attack,
		defence,
		health;
	std::string enemyType;
};

