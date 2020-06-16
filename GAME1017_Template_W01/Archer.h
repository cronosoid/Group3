#pragma once
#include "Enemies.h"

class Archer :public Enemies
{
public:
	Archer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator = nullptr);

	virtual void Update() override;
	virtual void Render() override;
	void attack();
private:
	int Status;
};