#include "Boss.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <SDL_image.h>

#include "MathManager.h"
#include"TextureManager.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "SoundManager.h"

const int MAXHEALTH = 1000;

const int BOSSDAMAGE = 30;
const int BOSSDEFENCE = 10;

const int DETECTDISTANCE = 2000;
const int STOPDISTANCE = 32;
const int ATTACKDISTANCE = 300;

const float WALKSPEED = 0.1;
const float RUNSPEED = 0.2;

const int MAXATTACKWAITTIME = 70; // in frames
const float ATTACKCOOLDOWN = 1.5;

const int SUMMONCD = 20 * 60;
const int ULTIMATECD = 10 * 60;

Boss::Boss(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator) :Enemies(s, d, r, t, animator)
{
	this->curStatus = PATROLING;
	this->health = MAXHEALTH;
	this->damage = BOSSDAMAGE;
	this->defence = BOSSDEFENCE;
	this->enemyType = "Boss";
	this->m_speed = WALKSPEED;

	this->m_pText = TextureManager::GetTexture("Swordman"); // Will be "Boss" texture

	this->m_summonCd = 10 * 60;
}

void Boss::HandleSpells()
{
	if (m_summonCd++ > SUMMONCD)
	{
		m_summonCd = 0;

		SDL_FPoint bossPos = {m_dst.x / 64, m_dst.y / 64 };
		
		for (int i = 1; i <= 3; i++)
		{
			EnemyType type = swordman;
			if (rand() % 2 == 0)
				type = archer;
			EnemyManager::CreateEnemy(type, bossPos.x - 2 + i * 2, 0, Engine::Instance().GetRenderer());
		}
	}
}

void Boss::Update()
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

	HandleSpells();
	
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
			this->m_speed = WALKSPEED;

			float curX;
			animator->getFace() == 0 ? curX = m_dst.x + m_dst.w + 5 : curX = m_dst.x - 5;
			
			float curY = m_dst.y + m_dst.h / 2;
			
			MapObject* nextObject = COMA::FindFirstObjectOnTheRay({ curX,curY }, { 0, 1 });

			if (nextObject)
			{
				float dist = COMA::SquareRectDistance(*nextObject->GetDstP(), *m_floor->GetDstP());

				if (dist < pow(m_floor->GetDstP()->w * 3 + 10, 2) and abs(nextObject->GetDstP()->y - m_floor->GetDstP()->y) < 32)
				{
					SetAccelX((1.0 - 2.0 * animator->getFace()) * m_speed);
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
	case SEEKING:
	{
		this->m_speed = RUNSPEED + (rand() % 10) / 10.0;

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

		if (nextObject and squareDistToPlayer > pow(STOPDISTANCE, 2))
		{
			SetAccelX(direction * m_speed);
			if (m_floor and nextObject->GetDstP()->y < m_floor->GetDstP()->y)
			{
				this->SetAccelY(-JUMPFORCE / 2);
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
		
	}
	break;
	case DEAD:
		break;
	default:
		break;
	}
}

void Boss::Render()
{

	SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);
}

void Boss::attack()
{
	SDL_FRect rect;
	if (this->getAnimator()->getFace() == 0)
	{
		rect.x = this->GetDstP()->x;
	}
	else
	{
		rect.x = this->GetDstP()->x - this->GetDstP()->w;
	}

	rect.y = this->GetDstP()->y;
	rect.w = this->GetDstP()->w * 2;
	rect.h = this->GetDstP()->h;

	if (COMA::AABBCheck(rect, *EnemyManager::GetTarget()->GetDstP())
		and static_cast<PlatformPlayer*>(EnemyManager::GetTarget())->GetLastAttackedTime() <= 0)
	{
		if (this->getAnimator()->getFace() == 0)
		{
			EnemyManager::GetTarget()->setHitDir(1);//set hit direction as left
		}
		else
		{
			EnemyManager::GetTarget()->setHitDir(2);//set hit direction as right
		}
		EnemyManager::GetTarget()->getDamage(BOSSDAMAGE);
		static_cast<PlatformPlayer*>(EnemyManager::GetTarget())->SetLastAttackedTime();
		EnemyManager::GetTarget()->StopX();
		SOMA::PlaySound("SwordHit");
		std::cout << "Boss Melee!\n";
	}
}

