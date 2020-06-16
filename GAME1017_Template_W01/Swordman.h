#pragma once
#include "Enemies.h"

//const int ATTACKCOOLDOWN = 1.5;

class Swordman :public Enemies
{
public:
	Swordman(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator = nullptr);

	virtual void Update() override;
	virtual void Render() override;
private:
	Uint32 lastAttackTime = 0;
};
