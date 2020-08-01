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
#include "MoveManager.h"

const int MAXHEALTH = 100;
const int ARCHERDAMAGE = 10;
const int ARCHERDEFENCE = 10;
const int DETECTDISTANCE = 550;
const int STOPDISTANCE = 100;
const int ATTACKDISTANCE = 300;
const float WALKSPEED = 0.4;
const float RUNSPEED = 0.55;

const int MAXATTACKWAITTIME = 30; // in frames

const float w = 128.0;
const float h = 128.0;

Archer::Archer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator) :Enemies(s, d, {0,0,w,h}, r, t, animator)
{
	this->curStatus = PATROLING;
	this->health = this->maxHealth = MAXHEALTH;
	this->damage = ARCHERDAMAGE;
	this->defence = ARCHERDEFENCE;
	this->enemyType = "Archer";
	this->m_speed = WALKSPEED;

	this->addAnimator(new Animator(this));

	this->getAnimator()->addAnimation("run", 4, 2, 64, 64);
	this->getAnimator()->addAnimation("idle", 3, 1, 64, 64, 0, 128, 12);
	this->getAnimator()->addAnimation("shot", 8, 3, 64, 64, 0, 256, 6);
}

void Archer::Update()
{	
	if (m_body.y >= MOMA::GetWindowY() - MOMA::GetTotalMove().y)
	{
		setActive(false);
	}

	static int attackWaitTime = 0;

	float squareDistToPlayer = COMA::SquareRectDistance(*this->GetDstP(), *EnemyManager::GetTarget()->GetDstP());
	if (curStatus != ATTACKING)
	{
		if (squareDistToPlayer < pow(DETECTDISTANCE, 2))
		{
			if (this->health > this->maxHealth * 0.5)
				curStatus = SEEKING;
			else
				curStatus = FLEEING;
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

			if (nextObject and not nextObject->getIsHurt() and squareDistToPlayer > pow(STOPDISTANCE,2))
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
		{
			
			this->m_speed = RUNSPEED + (rand() % 10) / 10.0;

			PlatformPlayer* player = EnemyManager::GetTarget();
			float dist = player->GetDstP()->x - this->m_dst.x;

			float direction = 1;
			if (dist != 0)
			{
				direction = abs(dist) / dist;
			}
			if (direction == 1)
				animator->setFace(1);
			else if (direction == -1)
				animator->setFace(0);

			float curX;
			animator->getFace() == 0 ? curX = m_dst.x + m_dst.w + 5 : curX = m_dst.x - 5;
			float curY = m_dst.y;
			MapObject* nextObject = COMA::FindFirstObjectOnTheRay({ curX,curY }, { 0, 1 });

			if (nextObject and not nextObject->getIsHurt() and squareDistToPlayer > pow(STOPDISTANCE, 2))
			{
				SetAccelX(-direction * m_speed);
				if (m_floor and nextObject->GetDstP()->y < m_floor->GetDstP()->y)
				{
					this->SetAccelY(-JUMPFORCE);
				}
			}

			if ((this->lastAttackTime + ATTACKCOOLDOWN * 1000) < SDL_GetTicks())
			{
				curStatus = ATTACKING;
			}
		}
		break;
	case ATTACKING:
		if ((this->lastAttackTime + ATTACKCOOLDOWN * 1000) < SDL_GetTicks())
		{
			this->getAnimator()->playFullAnimation("shot");
			float direction = 0;
			float dist = EnemyManager::GetTarget()->GetDstP()->x - this->m_dst.x;
			if (dist != 0)
			{
				direction = abs(dist) / dist;
			}
			if (direction == 1)
				animator->setFace(0);
			else if (direction == -1)
				animator->setFace(1);
			
			attackWaitTime = MAXATTACKWAITTIME;
			this->lastAttackTime = SDL_GetTicks();
			attack();
		}
		if (--attackWaitTime <= 0)
		{
			attackWaitTime = 0;
			if (this->health > this->maxHealth * 0.5)
				curStatus = SEEKING;
			else
				curStatus = FLEEING;
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

	if (this->GetVelX() == 0)
		this->getAnimator()->setNextAnimation("idle");
	else
		this->getAnimator()->setNextAnimation("run");

	movementUpdate();

	this->getAnimator()->update();
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
		{ face == 1 ? this->m_body.x + this->m_body.w : this->m_body.x - 24,
		this->m_body.y + 42, 48, 48 },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("Arrow"), 15, face, this->damage));
}