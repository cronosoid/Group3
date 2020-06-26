#include "States.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h" // Make sure this is NOT in "States.h" or circular reference.
#include "TextureManager.h"
#include "Engine.h"
#include "Button.h"
#include "Background.h"
#include "EnemyManager.h"
#include "Projectile.h"
#include "ProjectileManager.h"
#include "Utilities.h"
#include "UIObjectManager.h"

#include <iostream>

// Begin State. CTRL+M+H and CTRL+M+U to turn on/off collapsed code.
void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}
void State::Resume() {}
// End State.

ClickState::ClickState()
{
}

void ClickState::Update()
{
	if (EVMA::MousePressed(1))
	{
		STMA::ChangeState(new TitleState);
	}
}

void ClickState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 8, 0, 17, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	c_label->Render();
	State::Render();
}

void ClickState::Enter()
{
	c_label = new Background({ 0, 0, 500, 100 }, { 280.0f, 284.0f, 500.0f, 100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("click"));
}

void ClickState::Exit()
{
	delete c_label;
	c_label = nullptr;
}

// Begin TitleState.
TitleState::TitleState() {}

void TitleState::Enter()
{
	m_playBtn = new PlayButton({ 0,0,400,100 }, { 60.0f,350.0f,320.0f,80.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	SOMA::Load("Aud/button.wav", "button", SOUND_SFX);
	SOMA::Load("Aud/Fire.wav", "jump", SOUND_SFX);
	SOMA::Load("Aud/Kaben_jump.wav", "Kaben_jump", SOUND_SFX);
	SOMA::Load("Aud/bamboo.mp3", "bamboo", SOUND_MUSIC);
	SOMA::Load("Aud/horn.mp3", "horn", SOUND_MUSIC);
	SOMA::Load("Aud/ambient.mp3", "ambient", SOUND_MUSIC);
	TEMA::RegisterTexture("../Spritesheets/fireball.png", "fireball");
	SOMA::PlayMusic("horn", -1, 0);
	SOMA::SetMusicVolume(16);
	t_background = new Background({ 0, 0, 1024, 768 }, { 0.0f, 0.0f , 1024.0f , 768.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("background"));
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
	t_background->Render();
	m_playBtn->Render();
	State::Render();
}

void TitleState::Exit()
{
	std::cout << "Exiting TitleState..." << std::endl;
	
}
// End TitleState.

// Begin GameState.
GameState::GameState() {}

void GameState::Enter()
{
	std::cout << "Entering GameState..." << std::endl;
	SOMA::PlayMusic("bamboo", -1, 0);
	m_pPlayer = new PlatformPlayer({ 0,0,34,50 }, { 512.0f,480.0f,64.0f,100.0f },
								   Engine::Instance().GetRenderer(), 
									IMG_LoadTexture(Engine::Instance().GetRenderer(), "../Spritesheets/Kaben_Sheet.png"));
	m_pPlayer->addAnimator(new Animator(m_pPlayer));
	m_pPlayer->getAnimator()->addAnimation("run", 8, 2, 34, 50);
	m_pPlayer->getAnimator()->addAnimation("idle", 4, 1, 34, 50, 0, 100, 12);
	m_pPlatforms.push_back(new SDL_FRect({ 462.0f,648.0f,100.0f,20.0f }));

	m_pPlatforms.push_back(new SDL_FRect({ 220.0f,468.0f,100.0f,20.0f }));
	m_pPlatforms.push_back(new SDL_FRect({ 100.0f,468.0f,100.0f,20.0f }));
	
	m_pPlatforms.push_back(new SDL_FRect({ 724.0f,468.0f,100.0f,20.0f }));
	m_pPlatforms.push_back(new SDL_FRect({ 462.0f,368.0f,100.0f,20.0f }));
	m_pPlatforms.push_back(new SDL_FRect({ -100.0f,668.0f,1224.0f,100.0f }));
	EnemyManager::CreateEnemy(swordman, { 700.0f,300.0f,128.0f,128.0f }, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(archer, { 200.0f,300.0f,128.0f,128.0f }, Engine::Instance().GetRenderer());
	UIObjectManager::CreateSoulBar({ 50.0f,20.0f,256.0f,128.0f }, { 105.0f,72.0f,185.0f,20.0f }, Engine::Instance().GetRenderer(), m_pPlayer);
}

void GameState::Update()
{
	// Get input.
	m_pPlayer->movement[0] = 0;
	if (EVMA::KeyHeld(SDL_SCANCODE_A))
	{
		//walk left animation goes here
		m_pPlayer->getAnimator()->setFace(1);
		m_pPlayer->movement[0] = -1;
		m_pPlayer->getAnimator()->setNextAnimation("run");
		m_pPlayer->SetAccelX(-1.0);
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_D))
	{
		//walk right animation goes here
		m_pPlayer->getAnimator()->setFace(0);
		m_pPlayer->movement[0] = 1;
		m_pPlayer->getAnimator()->setNextAnimation("run");
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
			if (m_pPlayer->getAnimator()->getFace() == 0)
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
					enemy->getDamage(m_pPlayer->m_meeleDmg);
					std::cout << "Melee attacked!\n";
				}
			}

			m_pPlayer->Meele();
		}
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_I)) // fireball
	{
		if ((m_pPlayer->getMagicTime() + MAGICCOOLDOWN * 1000) < SDL_GetTicks())
		{
			m_pPlayer->setMagicTime();
			// will complete the projectile spawn in a while
			int face;
			m_pPlayer->getAnimator()->getFace() == 0 ? face = 1 : face = -1;
			ProMA::Instance().GetProjectiles().push_back(new Projectile({ 0,0,320,320 },
				{ face == 1 ? m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w : m_pPlayer->GetDstP()->x - 24,
				m_pPlayer->GetDstP()->y + 42, 48, 48 },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("fireball"), 20, face, m_pPlayer->m_magicDmg));
			m_pPlayer->ChangeSoul(-FIREBALLCOST);
		}
	}
	// Wrap the player on screen.
	if (m_pPlayer->GetDstP()->x < -51.0) m_pPlayer->SetX(1024.0);
	else if (m_pPlayer->GetDstP()->x > 1024.0) m_pPlayer->SetX(-50.0);
	// Do the rest.
	m_pPlayer->Update();
	for (auto projectile = ProMA::Instance().GetProjectiles().begin(); projectile != ProMA::Instance().GetProjectiles().end();)
	{
		(*projectile)->Update();
		if ((*projectile)->GetDstP()->x > 1024)
		{
			delete *projectile;
			projectile = ProMA::Instance().GetProjectiles().erase(projectile);
		}
		else
		{
			projectile++;
		}
	}
	if (m_pPlayer->movement[0] == 0)
		m_pPlayer->getAnimator()->setNextAnimation("idle");
	m_pPlayer->getAnimator()->playAnimation();

	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		enemy->Update();
	}
	EnemyManager::DestroyInvalidEnemies();

	CheckCollision();
	// Die
	if (m_pPlayer->GetSoul() <= 0)
		STMA::ChangeState(new EndState());
	if (EVMA::KeyHeld(SDL_SCANCODE_X))
	{
		STMA::ChangeState(new EndState());
	}

	UIObjectManager::UIUpdate();
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

	for (auto projectile : ProMA::Instance().GetProjectiles())
	{
		projectile->Render();
	}

	UIObjectManager::UIRender();

	// If GameState != current state.
	if (dynamic_cast<GameState*>(STMA::GetStates().back()))
		State::Render();
}

void GameState::Exit()
{
	for (const auto& mapElement : m_pPlayer->getAnimator()->animationsMap)
	{
		if (mapElement.second != nullptr)
		{
			delete mapElement.second;
		}
	}
	delete m_pPlayer->getAnimator();
	delete m_pPlayer;
	for (auto platfrom : m_pPlatforms)
	{
		delete platfrom;
	}
	UIObjectManager::DestroyUIObjects();
	for (auto enemy : EnemyManager::EnemiesVec)
	{
		enemy->setActive(false);
	}
	for (auto projectile = ProMA::Instance().GetProjectiles().begin(); projectile != ProMA::Instance().GetProjectiles().end();)
	{
		delete* projectile;
		projectile = ProMA::Instance().GetProjectiles().erase(projectile);
	}
	EnemyManager::DestroyInvalidEnemies();
}

void GameState::Resume() { }
// End GameState.

EndState::EndState()
{
}

void EndState::Enter()
{
	SOMA::PlayMusic("ambient", -1, 0);
	e_background = new Background({ 0, 0, 1024, 768 }, { 0.0f, 0.0f , 1024.0f , 768.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("end_background"));
	m_restartBtn = new RestartButton({ 0,0,400,100 }, { 312.0f,225.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("restart"));
	m_exitBtn = new ExitButton({ 0,0,400,100 }, { 312.0f,425.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("exit"));
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
	e_background->Render();
	m_restartBtn->Render();
	m_exitBtn->Render();
	State::Render();
}

void EndState::Exit()
{
	
}
