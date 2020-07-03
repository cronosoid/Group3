#include "Archer.h"

#include <ctime>

#include "MathManager.h"
#include "TextureManager.h"
#include "ProjectileManager.h"
#include <SDL_image.h>
#include <iostream>
#include "Arrow.h"

const int MAXHEALTH = 100;
const int ARCHERDAMAGE = 10;
const int ARCHERDEFENCE = 10;

Archer::Archer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, PlatformPlayer* hero, std::vector<MapObject*> mapObjects, Animator* animator) :Enemies(s, d, r, t, animator)
{
	curStatus = IDLE;
	health = MAXHEALTH;
	damage = ARCHERDAMAGE;
	defence = ARCHERDEFENCE;
	this->hero = hero;
	this->mapObjects = mapObjects;
	enemyType = "Archer";
}

void Archer::Update()
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
}

void Archer::Render()
{
	m_pText = TextureManager::GetTexture("Archer");
	
	SDL_RenderCopy(m_pRend, m_pText, &m_src, &MAMA::ConvertFRect2Rect(m_dst));
}

void Archer::attack()
{
	int face;
	this->animator->getFace() == 0 ? face = 1 : face = -1;
	PMA::Instance().GetProjectiles().push_back(new Arrow(hero, mapObjects,{ 0,0,320,320 },
		{ face == 1 ? this->GetDstP()->x + this->GetDstP()->w : this->GetDstP()->x - 24,
		this->GetDstP()->y + 42, 48, 48 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Arrow"), 15, face, this->damage));
}