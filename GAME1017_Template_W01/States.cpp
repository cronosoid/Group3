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
#include "MapObjectManager.h"
#include "MoveManager.h"
#include "Fireball.h"
#include <iostream>
#include <ctime>

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

int GameState::m_score=0;
int GameState::m_time=0;
int GameState::m_currentLevel=0;

GameState::GameState(Level* newLevel)
{
	m_pLevel = newLevel;
}

void GameState::Enter()
{
	std::cout << "Entering GameState..." << std::endl;

	SOMA::PlayMusic("bamboo", -1, 0);
	
	MapObjectManager::Init();
	UIObjectManager::Init();
	EnemyManager::Init();

	m_pPlayer = new PlatformPlayer({ 0,0,34,50 }, { 128.0f,600.0f,64.0f,100.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("KabenSheet"));

	m_pLevel->Load(m_pPlayer);

	EnemyManager::SetTarget(m_pPlayer);

	
	UIObjectManager::CreateSoulBar({ 50.0f,20.0f,256.0f,128.0f }, { 105.0f,72.0f,185.0f,20.0f }, Engine::Instance().GetRenderer(), m_pPlayer);

	m_MapDamageCounter = 0;
	m_currentLevel = m_pLevel->getLevelNum();
	m_score = 0;
	m_start = clock();
	std::cout << "Start: " << (int)m_start << std::endl;
}

void GameState::Update()
{

	MapObjectManager::Update();

	m_pPlayer->Update();

	PMA::Instance().Update();

	if (EnemyManager::GetBoss())
	{
		EnemyManager::GetBoss()->Update();
	}
	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		if (enemy->getType() != "Boss")
		{
			enemy->Update();
		}
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
	m_end=clock();
	m_time = (int)(m_end - m_start)/1000;
	std::cout << "End: " << (int)m_end << std::endl;
	std::cout << "Time: " << (int)m_time << std::endl;
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
		if (enemy != nullptr)
		{
			enemy->setActive(false);
		}
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
	SDL_RenderClear(Engine::Instance().GetRenderer());
	e_background->Render();
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
//End EndState

//Begin CongratulationState

CongratulationState::CongratulationState()
{
	
}

void CongratulationState::Enter()
{
	SOMA::PlayMusic("ambient", -1, 0);
	e_background = new Background({ 0, 0, 1024, 768 }, { 0.0f, 0.0f , 1024.0f , 768.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("end_background"));
	m_Congratulation = new Sprite({ 0,0,400,92 }, { 300.0f,150.0f,400.0f,92.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("congratulations"));
	switch (GameState::m_currentLevel)
	{
	case 1:
	{
		m_nextLevelBtn = new NextLevelButton({ 0,0,400,100 }, { 312.0f,400.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("nextLevel"), 2);
		break;
	}
	case 2:
	{
		m_nextLevelBtn = new NextLevelButton({ 0,0,400,100 }, { 312.0f,400.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("nextLevel"), 3);
		break;
	}
	case 3:
	{
		break;
	}
	default:break;
	}
	m_exitBtn = new ExitButton({ 0,0,400,100 }, { 312.0f,525.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("exit"));
	m_scoreLabel = new Label("Ltype", 450,260, " ");
	std::string Score = "Score: " + std::to_string(GameState::m_score);
	m_scoreLabel->SetText(Score.c_str());
	m_timeLabel = new Label("Ltype", 450, 300, " ");
	std::string Time = "Time: " + std::to_string(GameState::m_time) + "s";
	m_timeLabel->SetText(Time.c_str());
}

void CongratulationState::Update()
{
	if (m_nextLevelBtn != nullptr)
	{
		if (m_nextLevelBtn->Update() == 1)
			return;
	}	
	if (m_exitBtn->Update() == 1)
		return;
}

void CongratulationState::Render()
{
	SDL_RenderClear(Engine::Instance().GetRenderer());
	e_background->Render();
	m_Congratulation->Render();
	if(m_nextLevelBtn!=nullptr)
		m_nextLevelBtn->Render();
	m_exitBtn->Render();
	m_scoreLabel->Render();
	m_timeLabel->Render();
	State::Render();
}

void CongratulationState::Exit()
{
	delete m_Congratulation;
	m_Congratulation = nullptr;
	delete m_exitBtn;
	m_exitBtn = nullptr;
	if(m_nextLevelBtn!=nullptr)
	{
		delete m_nextLevelBtn;
		m_nextLevelBtn = nullptr;
	}
}

//End CongratulationState

