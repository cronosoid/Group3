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


const float w = 68.0;
const float h = 100.0;

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator)
	:Entity(s, d, { 0, 0, w, h}, r, t, animator, true)
{
	this->m_curSoul = m_maxSoul;

	this->animator = new Animator(this);
	
	this->addAnimator(new Animator(this));

	this->getAnimator()->addAnimation("run", 7, 2, 192, 64);
	this->getAnimator()->addAnimation("idle", 4, 1, 192, 64, 0, 128, 12);
	this->getAnimator()->addAnimation("melee", 7, 3, 192, 64,0,256,6);
	
	m_isUnderAttack = false;
	m_canControl = true;

	std::cout << "Player Created" << std::endl;
}

PlatformPlayer::~PlatformPlayer()
{
	if (this->getAnimator() != nullptr)
		delete this->getAnimator();
}

void PlatformPlayer::Update()
{
	if(m_lastAttacked>0)
	{
		setIsUnderAttack(true);
		if(m_lastAttacked>ATTACKINTERVAL-CANNOTCONTROLTIME)
			setCanControl(false);
	}
	if (m_stunTime > 0) m_stunTime--;
	
	if(m_canControl and m_stunTime == 0)
	{
		this->movement[0] = 0;
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			//walk left animation goes here
			if (EVMA::KeyPressed(SDL_SCANCODE_A))
			{
				SOMA::PlaySound("run", 1, 4);
			}
			this->getAnimator()->setFace(1);
			this->movement[0] = -1;
			this->getAnimator()->setNextAnimation("run");
			this->SetAccelX(-1.0);
		}
		if (EVMA::KeyReleased(SDL_SCANCODE_A))
		{
			Mix_Pause(4);
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			//walk right animation goes here
			if (EVMA::KeyPressed(SDL_SCANCODE_D))
			{
				SOMA::PlaySound("run", 1, 4);
			}
			this->getAnimator()->setFace(0);
			this->movement[0] = 1;
			this->getAnimator()->setNextAnimation("run");
			this->SetAccelX(1.0);
		}
		if (EVMA::KeyReleased(SDL_SCANCODE_D))
		{
			Mix_Pause(4);
		}
		if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && this->IsGrounded())
		{
			//jumping animation
			SOMA::PlaySound("jump");
			this->SetAccelY(-JUMPFORCE); // Sets the jump force.
			this->SetGrounded(false);
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_J)) //melee
		{
			if ((this->getMeleeTime() + MELEECOOLDOWN * 1000) < SDL_GetTicks())
			{
				
				this->getAnimator()->playFullAnimation("melee");
				this->setMeleeTime();
				SDL_FRect rect;
				if (this->getAnimator()->getFace() == 0)
				{
					rect.x = this->m_body.x + this->m_body.w;
				}
				else
				{
					rect.x = this->m_body.x - this->m_body.w;
				}

				rect.y = this->m_body.y;
				rect.w = this->m_body.w;
				rect.h = this->m_body.h;

				for (Enemies* enemy : EnemyManager::EnemiesVec)
				{
					if (COMA::AABBCheck(rect, *enemy->GetBody()))
					{
						this->SoulRecover();
						enemy->getDamage(this->m_meeleDmg);
						enemy->Stun(MELEESTUNTIME);
						SOMA::PlaySound("swordHit");
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
				SOMA::PlaySound("fireBall", 0, 1);
				PMA::Instance().GetProjectiles().push_back(new Fireball(this, EnemyManager::EnemiesVec, MapObjectManager::MapObjVec, { 0,0,39,32 },
					{ face == 1 ? this->GetBody()->x + this->GetBody()->w : this->GetBody()->x - 24,
					this->GetBody()->y + 42, 48, 48 },
					Engine::Instance().GetRenderer(), TEMA::GetTexture("fireball"), 15, face, this->m_magicDmg,
					4, 6, 39, 32));

				this->ChangeSoul(-FIREBALLCOST);
			}
		}

		if (this->movement[0] == 0)
			this->getAnimator()->setNextAnimation("idle");
		
	}
	
	
	if (m_lastAttacked > 0)
	{
		m_lastAttacked--;
		if (m_lastAttacked > ATTACKINTERVAL - STOPAFTERGOTATTACKED)
		{
			// this->getAnimator()->setNextAnimation("attacked");
			//Kaben will move back
			if (getHitDir()==1)
			{
				this->SetAccelX(1);
			}				
			else if (getHitDir() == 2)
			{
				this->SetAccelX(-1);
			}				
			
			if (this->IsGrounded())
			{
				this->SetAccelY(-15);
				this->SetGrounded(false);
				//std::cout << "Grounded: " <<std::endl;
			}				
			else
			{
				this->SetAccelY(1);
			}
		}
		if(m_lastAttacked <= ATTACKINTERVAL- CANNOTCONTROLTIME)//Invincibility should last a little longer than the player can't control
		{
			setCanControl(true);
		}
		if(m_lastAttacked==0)
		{
			setIsUnderAttack(false);
		}
	}

	this->getAnimator()->update();
	movementUpdate();
	//std::cout << "X: " << this->GetVelX() << " Y: " << this->GetVelY() << std::endl;
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

