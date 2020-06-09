#include "States.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h" // Make sure this is NOT in "States.h" or circular reference.
#include "TextureManager.h"
#include "Engine.h"
#include "Button.h"
#include "EnemyManager.h"

#include <iostream>

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
	
	EnemyManager::CreateEnemy(swordman, { 700.0f,300.0f,128.0f,128.0f }, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(archer, { 200.0f,300.0f,128.0f,128.0f }, Engine::Instance().GetRenderer());

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
		m_pPlayer->Fireball();
	}
	// Wrap the player on screen.
	if (m_pPlayer->GetDstP()->x < -51.0) m_pPlayer->SetX(1024.0);
	else if (m_pPlayer->GetDstP()->x > 1024.0) m_pPlayer->SetX(-50.0);
	// Do the rest.
	m_pPlayer->Update();
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
}

void GameState::CheckCollision()
{
	COMA::CheckPlatformsCollision(m_pPlatforms, m_pPlayer);

	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		COMA::CheckPlatformsCollision(m_pPlatforms, enemy);
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
