#pragma once
#include "Enemies.h"

enum SwordmanStatus{SmWaiting=1, SmSeeking, SmAttacking, SmDead, SmAttacked};

class Swordman:public Enemies
{
public:
	Swordman(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);

	virtual void Update() override;
	virtual void Render() override;

private:
	int Status;
	
};
