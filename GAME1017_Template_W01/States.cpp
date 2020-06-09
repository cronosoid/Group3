#include "States.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h" // Make sure this is NOT in "States.h" or circular reference.
#include "TextureManager.h"
#include "Engine.h"
#include "Button.h"
#include "EnemyManager.h"
#include "Fireball.h"
#include "ProjectileManager.h"
#include "Utilities.h"
#include <iostream>
#include "UIObjectManager.h"

// Begin State. CTRL+M+H and CTRL+M+U to turn on/off collapsed code.
void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}
void State::Resume() {}
// End State.

// Begin GameState.
GameState::GameState() {}

void GameState::Enter()
{
	std::cout << "Entering GameState..." << std::endl;
	m_pPlayer = new PlatformPlayer({ 0,0,64,64 }, { 512.0f,480.0f,128.0f,128.0f },
								   Engine::Instance().GetRenderer(), IMG_LoadTexture(Engine::Instance().GetRenderer(), "../Spritesheets/Kaben_Sheet.png"));
	m_pPlayerAnimator = new Animator(m_pPlayer);
	m_pPlayerAnimator->addAnimation("run", 8, 2, 64, 64);
	m_pPlayerAnimator->addAnimation("idle", 4, 1, 64, 64, 0, 128, 12);
	m_pPlatforms.push_back(new SDL_FRect({ 462.0f,648.0f,100.0f,20.0f }));
	m_pPlatforms.push_back(new SDL_FRect({ 200.0f,468.0f,100.0f,20.0f }));
	m_pPlatforms.push_back(new SDL_FRect({ 724.0f,468.0f,100.0f,20.0f }));
	m_pPlatforms.push_back(new SDL_FRect({ 462.0f,368.0f,100.0f,20.0f }));
	m_pPlatforms.push_back(new SDL_FRect({ -100.0f,668.0f,1224.0f,100.0f }));
	TEMA::RegisterTexture("../Spritesheets/fireball.png", "fireball");
	EnemyManager::CreateEnemy(swordman, { 700.0f,300.0f,128.0f,128.0f }, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(archer, { 200.0f,300.0f,128.0f,128.0f }, Engine::Instance().GetRenderer());
	UIObjectManager::CreateSoulBar({ 50.0f,20.0f,256.0f,128.0f }, Engine::Instance().GetRenderer(), m_pPlayer);
	SOMA::Load("Aud/Fire.wav", "jump", SOUND_SFX);
	SOMA::Load("Aud/Kaben_jump.wav", "Kaben_jump", SOUND_SFX);
}

void GameState::Update()
{
	// Get input.
	m_pPlayer->movement[0] = 0;
	if (EVMA::KeyHeld(SDL_SCANCODE_A))
	{
		//walk left animation goes here
		m_pPlayerAnimator->setFace(1);
		m_pPlayer->movement[0] = -1;
		m_pPlayerAnimator->setNextAnimation("run");
		m_pPlayer->SetAccelX(-1.0);
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_D))
	{
		//walk right animation goes here
		m_pPlayerAnimator->setFace(0);
		m_pPlayer->movement[0] = 1;
		m_pPlayerAnimator->setNextAnimation("run");
		m_pPlayer->SetAccelX(1.0);
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_pPlayer->IsGrounded())
	{
		//jumping animation
		SOMA::PlaySound("Kaben_jump");
		m_pPlayer->SetAccelY(-JUMPFORCE); // Sets the jump force.
		m_pPlayer->SetGrounded(false);
	}
	if (EVMA::KeyHeld(SDL_SCANCODE_J)) //melee
	{
		if ((m_pPlayer->getMeleeTime() + MELEECOOLDOWN * 1000) < SDL_GetTicks())
		{
			m_pPlayer->setMeleeTime();
			SDL_FRect rect;
			if (m_pPlayerAnimator->getFace() == 0)
			{
				rect.x = m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w;
			}
			else
			{
				rect.x = m_pPlayer->GetDstP()->x - m_pPlayer->GetDstP()->w;
			}
			
			rect.y = m_pPlayer->GetDstP()->y;
			rect.w = m_pPlayer->GetDstP()->w;
			rect.h = m_pPlayer->GetDstP()->h;

			for (Enemies* enemy : EnemyManager::EnemiesVec)
			{
				if (COMA::AABBCheck(rect, *enemy->GetDstP()))
				{
					m_pPlayer->SoulRcvry();
					enemy->damage(m_pPlayer->m_meeleDmg);
					std::cout << "Melee attacked!\n";
				}
			}

			m_pPlayer->Meele();
		}
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_I)) // fireball
	{
		// will complete the projectile spawn in a while
		ProMA::Instance().GetFireBalls().push_back(new Fireball({ 0,0,320,320, }, 
			{ m_pPlayer->GetDstP()->x + 40, m_pPlayer->GetDstP()->y + 42, 32, 32 }, 
			Engine::Instance().GetRenderer(), TEMA::GetTexture("fireball"), 10));
	}
	// Wrap the player on screen.
	if (m_pPlayer->GetDstP()->x < -51.0) m_pPlayer->SetX(1024.0);
	else if (m_pPlayer->GetDstP()->x > 1024.0) m_pPlayer->SetX(-50.0);
	// Do the rest.
	m_pPlayer->Update();
	for (int i = 0; i < ProMA::Instance().GetFireBalls().size(); i++)
	{
		ProMA::Instance().GetFireBalls()[i]->Update();
		if (ProMA::Instance().GetFireBalls()[i]->GetDstP()->x > 1024)
		{
			delete ProMA::Instance().GetFireBalls()[i];
			ProMA::Instance().GetFireBalls()[i] = nullptr;
			Engine::Instance().setNull();
		}
		if (Engine::Instance().isNull())
			CleanVector<Fireball*>(ProMA::Instance().GetFireBalls(), Engine::Instance().isNull());
	}
	if (m_pPlayer->movement[0] == 0)
		m_pPlayerAnimator->setNextAnimation("idle");
	m_pPlayerAnimator->playAnimation();

	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		enemy->Update();
	}
	EnemyManager::DestroyInvalidEnemies();

	CheckCollision();
	// Die
	if (m_pPlayer->GetSoul() <= 0)
		STMA::ChangeState(new EndState(m_pPlayer, m_pPlayerAnimator));
	if (EVMA::KeyHeld(SDL_SCANCODE_X))
	{
		STMA::ChangeState(new EndState(m_pPlayer, m_pPlayerAnimator));
	}

	UIObjectManager::UIUpdate();
}



void GameState::CheckCollision()
{
	COMA::CheckPlatformsCollision(m_pPlatforms, m_pPlayer);
	for (int i = 0; i < NUMPLATFORMS; i++) // For each platform.
	{
		if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_pPlatforms[i]))
		{
			if (m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w - m_pPlayer->GetVelX() <= m_pPlatforms[i]->x)
			{ // Collision from left.
				m_pPlayer->StopX(); // Stop the player from moving horizontally.
				m_pPlayer->SetX(m_pPlatforms[i]->x - m_pPlayer->GetDstP()->w);
			}
			else if (m_pPlayer->GetDstP()->x - (float)m_pPlayer->GetVelX() >= m_pPlatforms[i]->x + m_pPlatforms[i]->w)
			{ // Colliding right side of platform.
				m_pPlayer->StopX();
				m_pPlayer->SetX(m_pPlatforms[i]->x + m_pPlatforms[i]->w);
			}
			else if (m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h - (float)m_pPlayer->GetVelY() <= m_pPlatforms[i]->y)
			{ // Colliding top side of platform.
				m_pPlayer->SetGrounded(true);
				m_pPlayer->StopY();
				m_pPlayer->SetY(m_pPlatforms[i]->y - m_pPlayer->GetDstP()->h - 1);
			}
			else if (m_pPlayer->GetDstP()->y - (float)m_pPlayer->GetVelY() >= m_pPlatforms[i]->y + m_pPlatforms[i]->h)
			{ // Colliding bottom side of platform.
				m_pPlayer->StopY();
				m_pPlayer->SetY(m_pPlatforms[i]->y + m_pPlatforms[i]->h);
			}
		}
	}

	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		COMA::CheckPlatformsCollision(m_pPlatforms, enemy);
	}

	for (int i = 0; i < (int)EnemyManager::EnemiesVec.size(); i++)
	{
		for (int j = 0; j < NUMPLATFORMS; j++)
		{
			if (COMA::AABBCheck(*EnemyManager::EnemiesVec[i]->GetDstP(), *m_pPlatforms[j]))
			{
				if (EnemyManager::EnemiesVec[i]->GetDstP()->x + EnemyManager::EnemiesVec[i]->GetDstP()->w - EnemyManager::EnemiesVec[i]->GetVelX() <= m_pPlatforms[j]->x)
				{ // Collision from left.
					EnemyManager::EnemiesVec[i]->StopX(); // Stop the player from moving horizontally.
					EnemyManager::EnemiesVec[i]->SetX(m_pPlatforms[j]->x - EnemyManager::EnemiesVec[i]->GetDstP()->w);
				}
				else if (EnemyManager::EnemiesVec[i]->GetDstP()->x - (float)EnemyManager::EnemiesVec[i]->GetVelX() >= m_pPlatforms[j]->x + m_pPlatforms[j]->w)
				{ // Colliding right side of platform.
					EnemyManager::EnemiesVec[i]->StopX();
					EnemyManager::EnemiesVec[i]->SetX(m_pPlatforms[j]->x + m_pPlatforms[j]->w);
				}
				else if (EnemyManager::EnemiesVec[i]->GetDstP()->y + EnemyManager::EnemiesVec[i]->GetDstP()->h - (float)EnemyManager::EnemiesVec[i]->GetVelY() <= m_pPlatforms[j]->y)
				{ // Colliding top side of platform.
					EnemyManager::EnemiesVec[i]->SetGrounded(true);
					EnemyManager::EnemiesVec[i]->StopY();
					EnemyManager::EnemiesVec[i]->SetY(m_pPlatforms[j]->y - EnemyManager::EnemiesVec[i]->GetDstP()->h - 1);
				}
				else if (EnemyManager::EnemiesVec[i]->GetDstP()->y - (float)EnemyManager::EnemiesVec[i]->GetVelY() >= m_pPlatforms[j]->y + m_pPlatforms[j]->h)
				{ // Colliding bottom side of platform.
					EnemyManager::EnemiesVec[i]->StopY();
					EnemyManager::EnemiesVec[i]->SetY(m_pPlatforms[j]->y + m_pPlatforms[j]->h);
				}
			}
		}
	}
}


void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 128, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	// Draw the player.
	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		enemy->Render();
	}
	m_pPlayer->Render();
	// Draw the platforms.
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 192, 64, 0, 255);
	for (auto platfrom : m_pPlatforms)
	{
		SDL_RenderFillRectF(Engine::Instance().GetRenderer(), platfrom);
	}

	for (int i = 0; i < (int)ProMA::Instance().GetFireBalls().size(); i++)
	{
		ProMA::Instance().GetFireBalls()[i]->Render();
	}

	for (int i = 0; i < (int)EnemyManager::EnemiesVec.size(); i++)
	{
		EnemyManager::EnemiesVec[i]->Render();
	}
	UIObjectManager::UIRender();

	// If GameState != current state.
	if (dynamic_cast<GameState*>(STMA::GetStates().back()))
		State::Render();
}


void GameState::Exit()
{
	delete m_pPlayer;
	for (auto platfrom : m_pPlatforms)
	{
		delete platfrom;
	}
}

void GameState::Resume() { }
// End GameState.

// Begin TitleState.
TitleState::TitleState() {}

void TitleState::Enter()
{
	m_playBtn = new PlayButton({ 0,0,400,100 }, { 312.0f,100.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	SOMA::Load("Aud/button.wav", "button", SOUND_SFX);
}

void TitleState::Update()
{
	if (m_playBtn->Update() == 1)
		return; 
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 128, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	m_playBtn->Render();
	State::Render();
}

void TitleState::Exit()
{
	std::cout << "Exiting TitleState..." << std::endl;
}
// End TitleState.

EndState::EndState(PlatformPlayer* m_pPlayer, Animator* m_pPlayerAnimator)
{
	this->m_pPlayer = m_pPlayer;
	this->m_pPlayerAnimator = m_pPlayerAnimator;
}

void EndState::Update()
{
	if (m_restartBtn->Update() == 1)
		return;
	if (m_exitBtn->Update() == 1)
		return;
}

void EndState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 128, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	m_restartBtn->Render();
	m_exitBtn->Render();
	State::Render();
}

void EndState::Enter()
{
	for (const auto& mapElement : m_pPlayerAnimator->animationsMap)
	{
		if (mapElement.second != nullptr)
		{
			delete mapElement.second;
		}
	}
	m_restartBtn = new RestartButton({ 0,0,400,100 }, { 312.0f,100.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	m_exitBtn = new ExitButton({ 0,0,400,100 }, { 312.0f,300.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
}

void EndState::Exit()
{
	
}
