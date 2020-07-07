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
GameState::GameState() {}

void GameState::Enter()
{
	std::cout << "Entering GameState..." << std::endl;
  
	MapObjectManager::Init();
	EnemyManager::Init();

	//Create a test level
	
  
	m_pPlayer = new PlatformPlayer({ 0,0,34,50 }, { 128.0f,600.0f,64.0f,100.0f },
		Engine::Instance().GetRenderer(), TEMA::GetTexture("KabenSheet"));
	m_pPlayer->addAnimator(new Animator(m_pPlayer));
	m_pPlayer->getAnimator()->addAnimation("run", 8, 2, 34, 50);
	m_pPlayer->getAnimator()->addAnimation("idle", 4, 1, 34, 50, 0, 100, 12);

	LoadLevel_1();

	UIObjectManager::Init();
	UIObjectManager::CreateSoulBar({ 50.0f,20.0f,256.0f,128.0f }, { 105.0f,72.0f,185.0f,20.0f }, Engine::Instance().GetRenderer(), m_pPlayer);

	m_MapDamageCounter = 0;
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
			PMA::Instance().GetProjectiles().push_back(new Fireball(m_pPlayer, EnemyManager::EnemiesVec, MapObjectManager::MapObjVec, { 0,0,64,64 },
				{ face == 1 ? m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w : m_pPlayer->GetDstP()->x - 24,
				m_pPlayer->GetDstP()->y + 42, 48, 48 },
				Engine::Instance().GetRenderer(), TEMA::GetTexture("fireball"), 15, face, m_pPlayer->m_magicDmg,
				4, 6, 64, 64));
			
			m_pPlayer->ChangeSoul(-FIREBALLCOST);
		}
	}

	MapObjectManager::Update();
	
	// Wrap the player on screen.
	if (m_pPlayer->GetDstP()->x < -51.0) m_pPlayer->SetX(1024.0);
	else if (m_pPlayer->GetDstP()->x > 1024.0) m_pPlayer->SetX(-50.0);

	// Do the rest.
	m_pPlayer->Update();
	PMA::Instance().Update();

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
	if (COMA::CheckPortalCollision(MapObjectManager::MapObjVec, m_pPlayer))
		STMA::ChangeState(new EndState());
	if (EVMA::KeyHeld(SDL_SCANCODE_X))
	{
		STMA::ChangeState(new EndState());
	}

	UIObjectManager::UIUpdate();

	if (m_MapDamageCounter > 0)
		m_MapDamageCounter--;
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

	MapObjectManager::Render(true);
	// Draw the player.
	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		enemy->Render();
	}
	m_pPlayer->Render();
	
	// Draw the platforms.
	MapObjectManager::Render(false);

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

	delete m_pPlayer->getAnimator();
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

	for (auto projectile = PMA::Instance().GetProjectiles().begin(); projectile != PMA::Instance().GetProjectiles().end();)
	{
		delete* projectile;
		projectile = PMA::Instance().GetProjectiles().erase(projectile);
	}

}

void GameState::Resume() { }

void GameState::LoadLevel_1()
{
	for(int i=0;i<26;i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 11, Engine::Instance().GetRenderer());
	}
	MapObjectManager::CreateMapObject(kPlate, 10, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 13, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 29, 11,Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 30, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 31, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 34, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 35, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 36, 11, Engine::Instance().GetRenderer());
	for(int i=39;i<85;i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 11, Engine::Instance().GetRenderer());
	}
	
	for(int i=48;i<58;i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 10, Engine::Instance().GetRenderer());
	}
	for (int i = 49; i < 58; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 9, Engine::Instance().GetRenderer());
	}
	for (int i = 50; i < 58; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 8, Engine::Instance().GetRenderer());
	}
	MapObjectManager::CreateMapObject(kPlate, 51, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 52, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 56, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 57, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 52, 6, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 56, 6, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 64, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 70, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPortal, 81, 6, Engine::Instance().GetRenderer());

	MapObjectManager::CreateMapObject(kPlate, 3, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 4, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 5, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 6, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 7, 8, Engine::Instance().GetRenderer());

	EnemyManager::CreateEnemy(archer, 4, 5, Engine::Instance().GetRenderer(), m_pPlayer, MapObjectManager::MapObjVec);
	EnemyManager::CreateEnemy(swordman, 16,8, Engine::Instance().GetRenderer(),m_pPlayer,MapObjectManager::MapObjVec);
	EnemyManager::CreateEnemy(swordman, 23, 8, Engine::Instance().GetRenderer(), m_pPlayer, MapObjectManager::MapObjVec);
	EnemyManager::CreateEnemy(swordman, 44, 8, Engine::Instance().GetRenderer(), m_pPlayer, MapObjectManager::MapObjVec);
	EnemyManager::CreateEnemy(swordman, 53, 5, Engine::Instance().GetRenderer(), m_pPlayer, MapObjectManager::MapObjVec);
	EnemyManager::CreateEnemy(swordman, 58, 8, Engine::Instance().GetRenderer(), m_pPlayer, MapObjectManager::MapObjVec);
	EnemyManager::CreateEnemy(swordman, 72, 5, Engine::Instance().GetRenderer(), m_pPlayer, MapObjectManager::MapObjVec);
	EnemyManager::CreateEnemy(archer, 79, 8, Engine::Instance().GetRenderer(), m_pPlayer, MapObjectManager::MapObjVec);
}

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
