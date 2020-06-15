#include "Swordman.h"
#include <iostream>
#include "MathManager.h"
#include"TextureManager.h"
#include <SDL_image.h>

const int INIHEALTH = 100;
const int SWORDMANDAMAGE = 10;
const int SWORDMANDEFENCE = 10;
const int SWORDMANATKCD = 500;

Swordman::Swordman(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator) :Enemies(s, d, r, t, animator)
{
	Status = SmWaiting;
	health = INIHEALTH;
	damage = SWORDMANDAMAGE;
	defence = SWORDMANDEFENCE;
	attackCD = SWORDMANATKCD;
	enemyType = "Swordman";
}

void Swordman::Update()
{
	movementUpdate();
	if (m_dst.y >= 768)
	{
		setActive(false);
	}

	if (health <= 0)
	{
		setAlive(false);
		Status = SmDead;
	}
}

void Swordman::Render()
{
	m_pText = TextureManager::GetTexture("Swordman");
	
	SDL_RenderCopy(m_pRend, m_pText, &m_src, &MAMA::ConvertFRect2Rect(m_dst));
}
