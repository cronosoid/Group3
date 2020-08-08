#pragma once
#ifndef _BOSS_H_
#define _BOSS_H_

#include "BossHealthbar.h"
#include "Enemies.h"

enum Stage { FIGHTING, RAGE };

class Boss :public Enemies
{
public:
	Boss(SDL_FRect d, SDL_Renderer* r, Animator* animator = nullptr);
	~Boss();
	
	virtual void Update() override;
	virtual void Render() override;

	bool HandleSpells();
	void attack();

private:
	Uint32 lastAttackTime = 0;

	Uint16 m_summonCd;
	Uint16 m_ultimateCd;
	Uint16 m_ultimating;

	BossHealthbar* m_healthbar;

	Stage m_stage;
};

#endif