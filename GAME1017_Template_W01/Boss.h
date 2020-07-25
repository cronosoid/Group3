#pragma once
#ifndef _BOSS_H_
#define _BOSS_H_

#include "Enemies.h"

class Boss :public Enemies
{
public:
	Boss(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator = nullptr);

	virtual void Update() override;
	virtual void Render() override;

	void HandleSpells();
	void attack();

private:
	Uint32 lastAttackTime = 0;

	Uint16 m_summonCd;
	Uint16 m_ultimateCd;
};

#endif