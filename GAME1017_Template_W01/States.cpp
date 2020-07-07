#include "States.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h" // Make sure this is NOT in "States.h" or circular reference.
#include "TextureManager.h"
#include "Engine.h"
#include "Button.h"
#include "EnemyManager.h"
#include "Projectile.h"
#include "ProjectileManager.h"
#include "Utilities.h"
#include "UIObjectManager.h"
#include "MapObjectManager.h"
#include "MoveManager.h"
#include "Fireball.h"
#include <iostream>

// Begin State. CTRL+M+H and CTRL+M+U to turn on/off collapsed code.
void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}
void State::Resume() {}
// End State.

// Begin TitleState.
TitleState::TitleState() {}

void TitleState::Enter()
{
	m_playBtn = new PlayButton({ 0,0,400,100 }, { 312.0f,100.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	SOMA::Load("Aud/button.wav", "button", SOUND_SFX);
	SOMA::Load("Aud/Fire.wav", "jump", SOUND_SFX);
	SOMA::Load("Aud/Kaben_jump.wav", "Kaben_jump", SOUND_SFX);
	TEMA::RegisterTexture("../Spritesheets/fireball.png", "fireball");
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

// Begin GameState.
GameState::GameState(Level* newLevel)
{
	m_pLevel = newLevel;
}

void GameState::Enter()
{
	std::cout << "Entering GameState..." << std::endl;
  
	MapObjectManager::Init();
	UIObjectManager::Init();
	EnemyManager::Init();

	m_pPlayer = new PlatformPlayer({ 0,0,34,50 }, { 128.0f,600.0f,64.0f,100.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("KabenSheet"));

	m_pLevel->Load(m_pPlayer);

	EnemyManager::SetTarget(m_pPlayer);
	
	UIObjectManager::CreateSoulBar({ 50.0f,20.0f,256.0f,128.0f }, { 105.0f,72.0f,185.0f,20.0f }, Engine::Instance().GetRenderer(), m_pPlayer);

	m_MapDamageCounter = 0;
}

void GameState::Update()
{
	MapObjectManager::Update();

	m_pPlayer->Update();

	PMA::Instance().Update();

	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		enemy->Update();
	}
	EnemyManager::DestroyInvalidEnemies();
	
	CheckCollision();

	UIObjectManager::UIUpdate();

	if (m_MapDamageCounter > 0)
		m_MapDamageCounter--;
	
	// Die
	if (m_pPlayer->GetSoul() <= 0)
	{
		STMA::ChangeState(new EndState());
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_X))
	{
		STMA::ChangeState(new EndState());
	}
	else
	{
		m_pLevel->Update();
	}
}

void GameState::CheckCollision()
{
	COMA::CheckMapCollision(MapObjectManager::MapObjVec, m_pPlayer);

	if(m_MapDamageCounter==0)
	{
		//std::cout << "test damage" << std::endl;
		COMA::CheckPlayerMapDamage(MapObjectManager::MapObjVec, m_pPlayer);
		//std::cout << "end test" << std::endl;
		m_MapDamageCounter = MAPDAMAGECD;
	}
	
	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		COMA::CheckMapCollision(MapObjectManager::MapObjVec, enemy);
		COMA::CheckEnemyMapDamage(MapObjectManager::MapObjVec, enemy);
	}
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 128, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());

	MapObjectManager::Render(true); //Draw collidable platforms
	
	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		enemy->Render();
	}

	m_pPlayer->Render();
	
	MapObjectManager::Render(false); //Draw not collidable platforms

	for (Projectile* projectile : PMA::Instance().GetProjectiles())
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

	delete m_pPlayer;

	for (vector<MapObject*>::iterator obj = MapObjectManager::MapObjVec.begin(); obj != MapObjectManager::MapObjVec.end();)
	{
		delete* obj;
		obj = MapObjectManager::MapObjVec.erase(obj);
	}

	UIObjectManager::DestroyUIObjects();

	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		enemy->setActive(false);
	}
	EnemyManager::DestroyInvalidEnemies();
	EnemyManager::SetTarget(nullptr);

	for (auto projectile = PMA::Instance().GetProjectiles().begin(); projectile != PMA::Instance().GetProjectiles().end();)
	{
		delete* projectile;
		projectile = PMA::Instance().GetProjectiles().erase(projectile);
	}

	delete m_pLevel;
}

void GameState::Resume() {}

// End GameState.

EndState::EndState()
{
}

void EndState::Enter()
{
	m_restartBtn = new RestartButton({ 0,0,400,100 }, { 312.0f,100.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	m_exitBtn = new ExitButton({ 0,0,400,100 }, { 312.0f,300.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
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

void EndState::Exit()
{
	delete m_restartBtn;
	m_restartBtn = nullptr;
	delete m_exitBtn;
	m_exitBtn = nullptr;
}
