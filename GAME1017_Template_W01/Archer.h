#pragma once
#include <vector>
#include "Enemies.h"
#include "MapObject.h"
#include "PlatformPlayer.h"

const int ATTACKCOOLDOWN = 1.5;

class Archer :public Enemies
{
public:
	Archer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, PlatformPlayer* hero, std::vector<MapObject*> mapObjects, Animator* animator = nullptr);

	virtual void Update() override;
	virtual void Render() override;
	void attack();

	PlatformPlayer* hero;
	std::vector<MapObject*> mapObjects;
private:
	Uint32 lastAttackTime = 0;
};