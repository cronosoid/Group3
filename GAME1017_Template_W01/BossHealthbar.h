#pragma once
#ifndef _BOSSBAR_H_
#define _BOSSBAR_H_

#include "Enemies.h"
#include "Sprite.h"

class BossHealthbar : public Sprite
{
public:
	BossHealthbar(Enemies* boss);
	~BossHealthbar();

	void Update();
	virtual void Render() override;
	
private:
	Enemies* m_boss;
	Sprite* m_scale;
};

#endif