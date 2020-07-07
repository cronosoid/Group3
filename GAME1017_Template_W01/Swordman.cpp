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
	curStatus = PATROLING;
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
	case PATROLING:
		{
			if (m_floor)
			{
				float curX;
				animator->getFace() == 0 ? curX = m_dst.x + m_dst.w + 5 : curX = m_dst.x - 5;
				float curY = m_dst.y + m_dst.h / 2;
				MapObject* nextObject = COMA::FindFirstObjectOnTheRay({ curX,curY }, { 0, 1 });

				if (nextObject)
				{
					float dist = COMA::SquareRectDistance(*nextObject->GetDstP(), *m_floor->GetDstP());
					float speed = 0.5;

					if (dist < pow(m_floor->GetDstP()->w * 3 + 10, 2) and abs(nextObject->GetDstP()->y - m_floor->GetDstP()->y) < 32)
					{
						SetAccelX((1.0 - 2.0 * animator->getFace()) * speed);
					}
					else
					{
						animator->getFace() == 0 ? animator->setFace(1) : animator->setFace(0);
					}
				}
				else
				{
					animator->getFace() == 0 ? animator->setFace(1) : animator->setFace(0);
				}
			}
		}
		break;
	case DEAD:
		break;
	default:
		break;
	}
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

