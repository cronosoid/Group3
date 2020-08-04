#pragma once
#ifndef _BOSS_H_
#define _BOSS_H_

#include "BossHealthbar.h"
#include "Enemies.h"

class Boss :public Enemies
{
public:
	Boss(SDL_FRect d, SDL_Renderer* r, Animator* animator = nullptr);
	~Boss();
	
	virtual void Update() override;
	virtual void Render() override;

	void HandleSpells();
	void attack();

private:
	Uint32 lastAttackTime = 0;

	Uint16 m_summonCd;
	Uint16 m_ultimateCd;

	BossHealthbar* m_healthbar;
};

#endif