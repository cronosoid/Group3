#include "Archer.h"
#include "MathManager.h"
#include"TextureManager.h"
#include <SDL_image.h>
#include <iostream>

const int INIHEALTH = 100;
const int ARCHERDAMAGE = 10;
const int ARCHERDEFENCE = 10;
const int ARCHERATKCD = 500;

Archer::Archer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator) :Enemies(s, d, r, t, animator)
{
	Status = ArWaiting;
	health = INIHEALTH;
	damage = ARCHERDAMAGE;
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

void Archer::attack()
{
	/*int face;
	m_pPlayerAnimator->getFace() == 0 ? face = 1 : face = -1;
	ProMA::Instance().GetProjectiles().push_back(new Projectile({ 0,0,320,320 },
		{ face == 1 ? m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w : m_pPlayer->GetDstP()->x - 24,
		m_pPlayer->GetDstP()->y + 42, 48, 48 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("fireball"), 20, face, m_pPlayer->m_magicDmg));*/
}