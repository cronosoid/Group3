#include "Archer.h"

#include <ctime>

#include "MathManager.h"
#include "TextureManager.h"
#include "ProjectileManager.h"
#include "CollisionManager.h"
#include "EnemyManager.h"

#include <SDL_image.h>
#include <iostream>
#include "Arrow.h"

const int MAXHEALTH = 100;
const int ARCHERDAMAGE = 10;
const int ARCHERDEFENCE = 10;
const int DETECTDISTANCE = 550;
const int STOPDISTANCE = 100;
const int ATTACKDISTANCE = 300;
const float WALKSPEED = 0.4;
const float RUNSPEED = 0.55;

const int MAXATTACKWAITTIME = 30; // in frames

Archer::Archer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator) :Enemies(s, d, r, t, animator)
{
	this->curStatus = PATROLING;
	this->health = this->maxHealth = MAXHEALTH;
	this->damage = ARCHERDAMAGE;
	this->defence = ARCHERDEFENCE;
	this->enemyType = "Archer";
	this->m_speed = WALKSPEED;
}

void Archer::Update()
{	
	movementUpdate();
	if (m_dst.y >= 768)
	{
		setActive(false);
	}

	static int attackWaitTime = 0;

	float squareDistToPlayer = COMA::SquareRectDistance(*this->GetDstP(), *EnemyManager::GetTarget()->GetDstP());
	if (curStatus != ATTACKING)
	{
		if (squareDistToPlayer < pow(DETECTDISTANCE, 2))
		{
			curStatus = SEEKING;
		}
		else
		{
			curStatus = PATROLING;
		}
	}

	if (health <= 0)
	{
		setAlive(false);
		curStatus = DEAD;
	}

	if (m_stunTime > 0)
	{
		m_stunTime--;
		curStatus = STUNNED;
	}

	switch (curStatus)
	{
	case IDLE:
		{
			
		}
		break;
	case PATROLING:
		{
			if (m_floor)
			{
				/*static MapObject* oldObject = nullptr; // FOR DEBUG
				if (oldObject != nullptr)
				{
					oldObject->SetTexture(TEMA::GetTexture("plate"));
				}
				std::cout << "Face: " << animator->getFace() << "\n";*/
				this->m_speed = WALKSPEED;

				float curX;
				animator->getFace() == 0 ? curX = m_dst.x + m_dst.w + 5 : curX = m_dst.x - 5;
				float curY = m_dst.y + m_dst.h / 2;
				MapObject* nextObject = COMA::FindFirstObjectOnTheRay({ curX,curY }, { 0, 1 });

				//oldObject = nextObject; // FOR DEBUG

				if (nextObject)
				{
					//nextObject->SetTexture(TEMA::GetTexture("hBrick")); // FOR DEBUG

					float dist = COMA::SquareRectDistance(*nextObject->GetDstP(), *m_floor->GetDstP());
				
					if (dist < pow(m_floor->GetDstP()->w * 3 + 10, 2) and abs(nextObject->GetDstP()->y - m_floor->GetDstP()->y) < 32)
					{
						SetAccelX((1.0 - 2.0 * animator->getFace()) * m_speed);
					}
					else
					{
						//std::cout << dist << " vs " << pow(m_floor->GetDstP()->w * 3 + 10, 2) << "\n"; // FOR DEBUG
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
	case SEEKING:
		{
			this->m_speed = RUNSPEED + (rand()%10)/10.0;
			
			PlatformPlayer* player = EnemyManager::GetTarget();
			float dist = player->GetDstP()->x - this->m_dst.x;

			float direction = 0;
			if (dist != 0)
			{
				direction = abs(dist) / dist;
			}
			if (direction == 1)
				animator->setFace(0);
			else if (direction == -1)
				animator->setFace(1);

			float curX;
			animator->getFace() == 0 ? curX = m_dst.x + m_dst.w + 5 : curX = m_dst.x - 5;
			float curY = m_dst.y;
			MapObject* nextObject = COMA::FindFirstObjectOnTheRay({ curX,curY }, { 0, 1 });

			if (nextObject and squareDistToPlayer > pow(STOPDISTANCE,2))
			{
				SetAccelX(direction * m_speed);
				if (m_floor and nextObject->GetDstP()->y < m_floor->GetDstP()->y)
				{
					this->SetAccelY(-JUMPFORCE/2);
				}
			}

			if (squareDistToPlayer < pow(ATTACKDISTANCE, 2))
			{
				curStatus = ATTACKING;
			}
		}
		break;
	case FLEEING:
		break;
	case ATTACKING:
		if ((this->lastAttackTime + ATTACKCOOLDOWN * 1000) < SDL_GetTicks())
		{
			attackWaitTime = MAXATTACKWAITTIME;
			this->lastAttackTime = SDL_GetTicks();
			attack();
		}
		if (--attackWaitTime <= 0)
		{
			attackWaitTime = 0;
			curStatus = SEEKING;
		}
		break;
	case STUNNED:
		{
			// Play stunned animation
		}
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
	PMA::Instance().GetProjectiles().push_back(new Arrow(EnemyManager::GetTarget(), MapObjectManager::MapObjVec,{ 0,0,320,320 },
		{ face == 1 ? this->GetDstP()->x + this->GetDstP()->w : this->GetDstP()->x - 24,
		this->GetDstP()->y + 42, 48, 48 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Arrow"), 15, face, this->damage));
}