#include "PlatformPlayer.h"
#include "Engine.h"
#include <algorithm>
#include <iostream>
#include "TextureManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"
#include "ProjectileManager.h"
#include "Fireball.h"

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator)
	:Entity(s, d, r, t, animator, true)
{
	this->m_curSoul = m_maxSoul;

	this->addAnimator(new Animator(this));

	this->getAnimator()->addAnimation("run", 8, 2, 34, 50);
	this->getAnimator()->addAnimation("idle", 4, 1, 34, 50, 0, 100, 12);
	
	std::cout << "Player Created" << std::endl;
}

PlatformPlayer::~PlatformPlayer()
{
	if (this->getAnimator() != nullptr)
		delete this->getAnimator();
}

void PlatformPlayer::Update()
{
	this->movement[0] = 0;
	if (EVMA::KeyHeld(SDL_SCANCODE_A))
	{
		//walk left animation goes here
		this->getAnimator()->setFace(1);
		this->movement[0] = -1;
		this->getAnimator()->setNextAnimation("run");
		this->SetAccelX(-1.0);
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_D))
	{
		//walk right animation goes here
		this->getAnimator()->setFace(0);
		this->movement[0] = 1;
		this->getAnimator()->setNextAnimation("run");
		this->SetAccelX(1.0);
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && this->IsGrounded())
	{
		//jumping animation
		SOMA::PlaySound("Kaben_jump");
		this->SetAccelY(-JUMPFORCE); // Sets the jump force.
		this->SetGrounded(false);
	}
	if (EVMA::KeyHeld(SDL_SCANCODE_J)) //melee
	{
		if ((this->getMeleeTime() + MELEECOOLDOWN * 1000) < SDL_GetTicks())
		{
			this->setMeleeTime();
			SDL_FRect rect;
			if (this->getAnimator()->getFace() == 0)
			{
				rect.x = this->GetDstP()->x + this->GetDstP()->w;
				
			}
			else
			{
				rect.x = this->GetDstP()->x - this->GetDstP()->w;
				
			}

			rect.y = this->GetDstP()->y;
			rect.w = this->GetDstP()->w;
			rect.h = this->GetDstP()->h;

			for (Enemies* enemy : EnemyManager::EnemiesVec)
			{
				if (COMA::AABBCheck(rect, *enemy->GetDstP()))
				{
					this->SoulRecover();
					enemy->getDamage(this->m_meeleDmg);
					enemy->Stun(MELEESTUNTIME);
					std::cout << "Melee attacked!\n";
				}
			}
			this->Meele();
		}
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_I)) // fireball
	{
		if ((this->getMagicTime() + MAGICCOOLDOWN * 1000) < SDL_GetTicks())
		{
			this->setMagicTime();
			// will complete the projectile spawn in a while
			int face;
			this->getAnimator()->getFace() == 0 ? face = 1 : face = -1;
			PMA::Instance().GetProjectiles().push_back(new Fireball(this, EnemyManager::EnemiesVec, MapObjectManager::MapObjVec, { 0,0,64,64 },
				{ face == 1 ? this->GetDstP()->x + this->GetDstP()->w : this->GetDstP()->x - 24,
				this->GetDstP()->y + 42, 48, 48 },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("fireball"), 15, face, this->m_magicDmg,
				4, 6, 39, 32));

			this->ChangeSoul(-FIREBALLCOST);
		}
	}

	if (EVMA::KeyPressed(SDL_SCANCODE_L))
	{
		if ((this->GetLastDashTime() + DASHCOOLDOWN * 1000) < SDL_GetTicks())
		{
			this->SetLastDashTime();
			StopY();
			if (getAnimator()->getFace() == 0)
			{
				m_accelX += 150;
			}
			else
			{
				m_accelX -= 150;
			}
		}
	}
	
	if (this->movement[0] == 0)
		this->getAnimator()->setNextAnimation("idle");
	
	if (m_lastAttacked > 0)
	{
		m_lastAttacked--;
		if (m_lastAttacked > ATTACKINTERVAL - STOPAFTERGOTATTACKED)
		{
			// this->getAnimator()->setNextAnimation("attacked");
			this->SetAccelX(0);
		}
	}

	this->getAnimator()->playAnimation();
	movementUpdate();
}

void PlatformPlayer::Render()
{
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
	//SDL_RenderFillRectF(Engine::Instance().GetRenderer(), GetDstP());
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, SDL_FLIP_NONE);
}

double PlatformPlayer::getHealPercent()
{
	return m_curSoul / m_maxSoul;
}

double PlatformPlayer::GetSoul()
{
	return m_curSoul;
}

void PlatformPlayer::SoulRecover()
{
	m_curSoul += m_soulRecover;
	if (m_curSoul > m_maxSoul)
		m_curSoul = m_maxSoul;
}

void PlatformPlayer::ChangeSoul(int amnt)
{
	m_curSoul += amnt;
	if (m_curSoul > m_maxSoul)
		m_curSoul = m_maxSoul;
	else if (m_curSoul < 0)
		m_curSoul = 0;
}

void PlatformPlayer::Meele()
{

	std::cout << "meele\n";

}

void PlatformPlayer::getDamage(int dmg)
{
	m_curSoul -= dmg;
}

