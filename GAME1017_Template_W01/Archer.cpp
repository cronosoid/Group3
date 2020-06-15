#include "Archer.h"

#include <iostream>

#include "MathManager.h"
#include"TextureManager.h"
#include <SDL_image.h>

const int INIHEALTH = 100;
const int ARCHERATTACK = 10;
const int ARCHERDEFENCE = 10;
const int ARCHERATKCD = 500;

Archer::Archer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :Enemies(s, d, r, t)
{
	Status = ArWaiting;
	health = INIHEALTH;
	attack = ARCHERATTACK;
	defence = ARCHERDEFENCE;
	attackCD = ARCHERATKCD;
	enemyType = "Archer";
}

void Archer::Update()
{
	movementUpdate();
	if (m_dst.y >= 768)
	{
		setActive(false);
	}
	if (health <= 0)
	{
		setAlive(false);
		Status = ArDead;
	}
}

void Archer::Render()
{
	m_pText = TextureManager::GetTexture("Archer");
	
	SDL_RenderCopy(m_pRend, m_pText, &m_src, &MAMA::ConvertFRect2Rect(m_dst));
}
