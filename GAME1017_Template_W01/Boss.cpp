#include "Boss.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <SDL_image.h>

#include "MathManager.h"
#include"TextureManager.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "MoveManager.h"
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

const int SUMMONCD = 20 * FPS;
const int PRESUMMONTIME = FPS * 2;

const float RAGEPERCENTAGE = 0.6;
const int ULTIMATECD = 5 * 60;
const int PREULTIMATETIME = FPS * 2;
const int ULTIMATETOSS = -60;
const float ULTIMATEDAMAGE = 45.0;

const float w = 200.0;
const float h = 256.0;

Boss::Boss(SDL_FRect d, SDL_Renderer* r, Animator* animator) :Enemies({ 0,0,64,64 }, d, {0,0,w,h}, r, TEMA::GetTexture("Boss"), animator)
{
	this->curStatus = PATROLING;
	this->health = this->maxHealth = MAXHEALTH;
	this->damage = BOSSDAMAGE;
	this->defence = BOSSDEFENCE;
	this->enemyType = "Boss";
	this->m_speed = WALKSPEED;

	this->m_maxJumpVelocity = 30;

	this->m_healthbar = new BossHealthbar(this);
	
	this->m_summonCd = 10 * 60;
	this->m_ultimateCd = ULTIMATECD - 8 * 4;
	
	this->m_stage = FIGHTING;

	this->addAnimator(new Animator(this));

	this->getAnimator()->addAnimation("run", 4, 2, 64, 64,0,0,16);
	this->getAnimator()->addAnimation("idle", 4, 1, 64, 64, 0, 128, 16);
	this->getAnimator()->addAnimation("punch", 4, 3, 64, 64, 0, 256, 10);
	this->getAnimator()->addAnimation("ground_slam", 8, 3, 64, 64, 0, 384, 8);
	this->getAnimator()->addAnimation("summon", 4, 3, 64, 64, 0, 512, 16);
	this->getAnimator()->addAnimation("ultimate_prepare", 4, 3, 64, 64, 0, 768, 30);
	this->getAnimator()->addAnimation("dying", 14, 4, 64, 64, 0, 640, 8);
}

Boss::~Boss()
{
	delete m_healthbar;
}


bool Boss::HandleSpells()
{
	if (m_summonCd++ > SUMMONCD and m_ultimateCd < ULTIMATECD)
	{
		if (m_summonCd < SUMMONCD + PRESUMMONTIME)
		{
			this->getAnimator()->setNextAnimation("summon");
			this->Stop();
		}
		else
		{
			m_summonCd = 0;

			SDL_FPoint bossPos = { m_dst.x / 64, m_dst.y / 64 };

			for (int i = 1; i <= 3; i++)
			{
				EnemyType type = swordman;
				if (rand() % 2 == 0)
					type = archer;
				EnemyManager::CreateEnemy(type, bossPos.x - 2 + i * 2, 0, Engine::Instance().GetRenderer());
			}
		}
		return true;
	}
	if (m_stage == RAGE)
	{
		if (++m_ultimateCd >= ULTIMATECD)
		{
			if (m_ultimateCd == ULTIMATECD)
			{
				this->getAnimator()->playFullAnimation("ultimate_prepare");
				m_ultimating = 0;
			}
			if (not this->getAnimator()->AnimationIsPlaying("ultimate_prepare"))
			{
				if (m_ultimating == 0)
				{
					this->getAnimator()->playFullAnimation("ground_slam");
				}
				if (++m_ultimating == 8 * 4)
				{
					StopX();

					m_ultimating = 0;
					m_ultimateCd = 0;

					for (Enemies* enemy : EnemyManager::EnemiesVec)
					{
						if (enemy != this and enemy->GetBody()->y < m_body.y + m_body.h and enemy->GetBody()->y + enemy->GetBody()->h > m_body.y)
						{
							enemy->SetAccelY(ULTIMATETOSS * (1 - 0.1 * (rand()%3)));
							enemy->Stun(60);
						}
					}

					PlatformPlayer* player = EnemyManager::GetTarget();
					if (player->GetBody()->y < m_body.y + m_body.h and player->GetBody()->y + player->GetBody()->h > m_body.y + m_body.h * (0.7))
					{
						player->SetAccelY(ULTIMATETOSS * (1 - 0.1 * (rand() % 3)));
						player->Stun(60);
						player->getDamage(ULTIMATEDAMAGE);
					}
				}
			}
			return true;
		}
	}
	return false;
}

void Boss::Update()
{
	m_healthbar->Update();
	
	/*if (m_dst.y >= MOMA::GetWindowY() - MOMA::GetTotalMove().y)
	{
		setActive(false);
	}*/

	static int attackWaitTime = 0;

	float squareDistToPlayer = COMA::SquareRectDistance(*this->GetDstP(), *EnemyManager::GetTarget()->GetDstP());

	if (not HandleSpells() or health <= 0)
	{
		if (curStatus != ATTACKING and curStatus != DYING)
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

		if (health <= 0 and curStatus != DYING)
		{
			m_dyingAnimation = (this->getAnimator()->GetAnimation("dying")->getMaxFrames() - 6)
                * this->getAnimator()->GetAnimation("dying")->getFramesFrequency() / 10;
			this->getAnimator()->playFullAnimation("dying");
			curStatus = DYING;
		}
		else if (health <= maxHealth * RAGEPERCENTAGE)
		{
			m_stage = RAGE;
		}

		if (m_stunTime > 0 and curStatus != DYING)
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
				Seek(RUNSPEED, squareDistToPlayer, STOPDISTANCE, ATTACKDISTANCE, true);
				if (squareDistToPlayer < pow(ATTACKDISTANCE, 2))
				{
					if ((this->lastAttackTime + ATTACKCOOLDOWN * 1000) < SDL_GetTicks())
					{
						curStatus = ATTACKING;
					}
				}
			}
			break;
		case FLEEING:
			{
		
			}
			break;
		case ATTACKING:
            if ((this->lastAttackTime + ATTACKCOOLDOWN * 1000) < SDL_GetTicks())
            {
            	this->getAnimator()->playFullAnimation("punch");
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
		case DYING:
			{
				if (m_dyingAnimation-- <= 0)
				{
					setAlive(false);
					EnemyManager::DestroyBoss();
					curStatus = DEAD;
				}
				break;
			}
		case DEAD:
            std::cout << "Dead";
		
			break;
		default:
            break;
		}
	}
	
	if (this->GetVelX() == 0)
		this->getAnimator()->setNextAnimation("idle");
	else
		this->getAnimator()->setNextAnimation("run");

	movementUpdate();
	
	this->getAnimator()->update();
}

void Boss::Render()
{
	m_healthbar->Render();
	SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);
}

void Boss::attack()
{
	SDL_FRect rect;
	if (this->getAnimator()->getFace() == 0)
	{
		rect.x = m_body.x;
	}
	else
	{
		rect.x = m_body.x - m_body.w * 0.55;
	}

	rect.y = m_body.y;
	rect.w = m_body.w * 1.55;
	rect.h = m_body.h;

	if (COMA::AABBCheck(rect, *EnemyManager::GetTarget()->GetBody())
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
		SOMA::PlaySound("swordHit");
		std::cout << "Boss Melee!\n";
	}
}

