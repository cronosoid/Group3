#include "Archer.h"
#include "MathManager.h"
#include "TextureManager.h"
#include "ProjectileManager.h"
#include "CollisionManager.h"

#include <SDL_image.h>
#include <iostream>

const int MAXHEALTH = 100;
const int ARCHERDAMAGE = 10;
const int ARCHERDEFENCE = 10;

Archer::Archer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator) :Enemies(s, d, r, t, animator)
{
	curStatus = PATROLING;
	health = MAXHEALTH;
	damage = ARCHERDAMAGE;
	defence = ARCHERDEFENCE;
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
		curStatus = DEAD;
	}
	else if (health < MAXHEALTH)
	{
		curStatus = ATTACKING;
	}

	switch (curStatus)
	{
	case IDLE:
		break;
	case PATROLING:
		{
		if (m_floor)
		{
			float curX;
			std::cout << "Face: " << animator->getFace() << "\n";
			animator->getFace() == 0 ? curX = m_dst.x + m_dst.w + 5 : curX = m_dst.x - 5;
			float curY = m_dst.y + m_dst.h / 2;
			MapObject* nextObject = COMA::FindFirstObjectOnTheRay({ curX,curY }, { 0, 1 });
			if (nextObject)
			{
				float dist = COMA::SquareRectDistance(*nextObject->GetDstP(), *m_floor);
				float speed = 0.2;
				
				if (dist < m_floor->w * 3 + 10)
				{
					SetAccelX((1.0 - 2.0 * animator->getFace()) * speed);
				}
				else
				{
					std::cout << dist << " vs " << m_floor->w * 2 + 10 << "\n";
					animator->getFace() == 0 ? animator->setFace(1) : animator->setFace(0);

					//nextObject->SetDstSize(60,60);
				}
			}
		}
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
	PMA::Instance().GetProjectiles().push_back(new Projectile({ 0,0,320,320 },
		{ face == 1 ? this->GetDstP()->x + this->GetDstP()->w : this->GetDstP()->x - 24,
		this->GetDstP()->y + 42, 48, 48 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Arrow"), 15, face, this->damage));
}