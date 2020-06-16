#pragma once
#include "Enemies.h"

class Swordman :public Enemies
{
public:
	Swordman(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator = nullptr);

	virtual void Update() override;
	virtual void Render() override;

private:
	int Status;
};
