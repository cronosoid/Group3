#pragma once
#include "Enemies.h"
#include "PlatformPlayer.h"

class Swordman :public Enemies
{
public:
	Swordman(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator = nullptr);

	virtual void Update() override;
	virtual void Render() override;
	void attack();
	Uint32 getAttackTime() { return lastAttackTime; }
	void setAttackTime() { lastAttackTime = SDL_GetTicks(); }
private:
	Uint32 lastAttackTime = 0;
};
