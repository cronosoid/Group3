#include "Swordman.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "MathManager.h"
#include"TextureManager.h"
#include <SDL_image.h>


#include "Archer.h"
#include "CollisionManager.h"

const int MAXHEALTH = 100;
const int SWORDMANDAMAGE = 10;
const int SWORDMANDEFENCE = 10;

Swordman::Swordman(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, PlatformPlayer* hero, Animator* animator) :Enemies(s, d, r, t, animator)
{
	curStatus = IDLE;
	health = MAXHEALTH;
	damage = SWORDMANDAMAGE;
	defence = SWORDMANDEFENCE;
	enemyType = "Swordman";
	this->hero = hero;
}

void Swordman::Update()
{
	srand((unsigned)time(NULL));
	int secRan = rand() % 3 + 1;
	
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
Liqi_Week7Latest
	else if (health < MAXHEALTH)
	{
		curStatus = ATTACKING;
	}

	switch (curStatus)
	{
	case IDLE:		
		if ((this->lastAttackTime + ATTACKCOOLDOWN * 1000 * secRan) < SDL_GetTicks())
		{
			this->lastAttackTime = SDL_GetTicks();
			attack();
		}
		
		break;
	case SEEKING:
		break;
	case FLEEING:
		break;
	case ATTACKING:
		if ((this->lastAttackTime + ATTACKCOOLDOWN * 1000) < SDL_GetTicks())
		{
			this->lastAttackTime = SDL_GetTicks();
			attack();
		}
		break;
	case DEAD:
		break;
	default:
		break;
	}
=======


latest_copy
}

void Swordman::Render()
{
	m_pText = TextureManager::GetTexture("Swordman");
	
	SDL_RenderCopy(m_pRend, m_pText, &m_src, &MAMA::ConvertFRect2Rect(m_dst));
}

void Swordman::attack()
{
	SDL_FRect rect;
	//if (getAnimator()->getFace() == 0)
	//{
	//	rect.x = GetDstP()->x + GetDstP()->w;
	//}
	//else
	//{
	//	rect.x = GetDstP()->x - GetDstP()->w;
	//}
	
	rect.x = GetDstP()->x - GetDstP()->w;
	rect.y = GetDstP()->y;
	rect.w = GetDstP()->w;
	rect.h = GetDstP()->h;
		
	if (COMA::AABBCheck(rect, *hero->GetDstP()))
	{
		hero->getDamage(SWORDMANDAMAGE);
		std::cout << "Swordman Melee!\n";
	}
}

