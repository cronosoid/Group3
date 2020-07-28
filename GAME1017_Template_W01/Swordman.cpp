#include "Swordman.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <SDL_image.h>
#include "MathManager.h"
#include"TextureManager.h"
#include "Archer.h"
#include "CollisionManager.h"
#include "ProjectileManager.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "MoveManager.h"
#include "SoundManager.h"

const int MAXHEALTH = 100;
const int SWORDMANDAMAGE = 10;
const int SWORDMANDEFENCE = 10;
const int DETECTDISTANCE = 400;
const int STOPDISTANCE = 32;
const int ATTACKDISTANCE = 100;
const float WALKSPEED = 0.4;
const float RUNSPEED = 0.55;

const int MAXATTACKWAITTIME = 30; // in frames

const float w = 128.0;
const float h = 128.0;

Swordman::Swordman(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator) :Enemies(s, d, {0,0,w,h}, r, t, animator)
{
	this->curStatus = PATROLING;
	this->health = this->maxHealth = MAXHEALTH;
	this->damage = SWORDMANDAMAGE;
	this->defence = SWORDMANDEFENCE;
	this->enemyType = "Swordman";
	this->m_speed = WALKSPEED;

	this->addAnimator(new Animator(this));
	
	this->getAnimator()->addAnimation("run", 4, 2, 64, 64);
	this->getAnimator()->addAnimation("idle", 3, 1, 64, 64, 0, 128, 12);
	this->getAnimator()->addAnimation("melee", 9, 3, 64, 64, 0, 256, 4);
}

void Swordman::Update()
{
	if (m_body.y >= MOMA::GetWindowY() - MOMA::GetTotalMove().y)
	{
		setActive(false);
	}

	static int attackWaitTime = 0;

	float squareDistToPlayer = COMA::SquareRectDistance(this->m_body, *EnemyManager::GetTarget()->GetDstP());
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
			animator->getFace() == 0 ? curX = m_body.x + m_body.w + 5 : curX = m_body.x - 5;
			float curY = m_body.y + m_body.h / 2;
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
		this->m_speed = RUNSPEED + (rand() % 10) / 10.0;

		PlatformPlayer* player = EnemyManager::GetTarget();
		float dist = player->GetBody()->x - this->m_body.x;

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
		animator->getFace() == 0 ? curX = m_body.x + m_body.w + 5 : curX = m_body.x - 5;
		float curY = m_body.y;
		MapObject* nextObject = COMA::FindFirstObjectOnTheRay({ curX,curY }, { 0, 1 });
		
		if (nextObject and not nextObject->getIsHurt() and squareDistToPlayer > pow(STOPDISTANCE, 2))
		{
			SetAccelX(direction * m_speed);
			if (m_floor and nextObject->GetDstP()->y < m_floor->GetDstP()->y)
			{
				this->SetAccelY(-JUMPFORCE);
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
			//std::cout << "Attacked\n";
			this->getAnimator()->playFullAnimation("melee");
			attackWaitTime = MAXATTACKWAITTIME;
			this->lastAttackTime = SDL_GetTicks();
			attack();
		}
		if (--attackWaitTime <= 0)
		{
			//std::cout << "Seeking\n";
			attackWaitTime = 0;
			curStatus = SEEKING;
		}
		break;
	case STUNNED:
	{
		// Play stunned animation
	}
	break;
	case DEAD:
		break;
	default:
		break;
	}

	if (this->GetVelX() == 0)
		this->getAnimator()->setNextAnimation("idle");
	else
		this->getAnimator()->setNextAnimation("run");
	
	movementUpdate();

	this->getAnimator()->update();
	this->getAnimator()->playAnimation();
}

void Swordman::Render()
{
	m_pText = TextureManager::GetTexture("Swordman");

	SDL_RenderCopy(m_pRend, m_pText, &m_src, &MAMA::ConvertFRect2Rect(m_dst));
}

void Swordman::attack()
{
	SDL_FRect rect;
	if (this->getAnimator()->getFace() == 0)
	{
		rect.x = m_body.x;
	}
	else
	{
		rect.x = m_body.x - m_body.w;
	}

	rect.y = m_body.y;
	rect.w = m_body.w * 2;
	rect.h = m_body.h;

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
		EnemyManager::GetTarget()->getDamage(SWORDMANDAMAGE);
		static_cast<PlatformPlayer*>(EnemyManager::GetTarget())->SetLastAttackedTime();
		EnemyManager::GetTarget()->StopX();
		SOMA::PlaySound("SwordHit");
		std::cout << "Swordman Melee!\n";
	}
}

