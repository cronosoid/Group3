#include "Swordman.h"
#include <iostream>
#include "MathManager.h"
#include"TextureManager.h"
#include <SDL_image.h>

const int MAXHEALTH = 100;
const int SWORDMANDAMAGE = 10;
const int SWORDMANDEFENCE = 10;

Swordman::Swordman(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator) :Enemies(s, d, r, t, animator)
{
	curStatus = IDLE;
	health = MAXHEALTH;
	damage = SWORDMANDAMAGE;
	defence = SWORDMANDEFENCE;
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
		curStatus = DEAD;
	}
}

void Swordman::Render()
{
	m_pText = TextureManager::GetTexture("Swordman");
	
	SDL_RenderCopy(m_pRend, m_pText, &m_src, &MAMA::ConvertFRect2Rect(m_dst));
}
