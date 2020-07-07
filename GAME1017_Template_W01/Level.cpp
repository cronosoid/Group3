#include "Level.h"
#include "MoveManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"
#include "StateManager.h"

Level::Level() {}

Level1::Level1()
{
	m_levelWidth = 4416;
	m_levelHight = 1300;
}

void Level1::Load(PlatformPlayer* Player)
{
	MoveManager::Init(Engine::Instance().GetWindow(), m_levelWidth, m_levelHight);

	this->m_pPlayer = Player;

	for (int i = 0; i < 26; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 11, Engine::Instance().GetRenderer());
	}
	MapObjectManager::CreateMapObject(kPlate, 10, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 16, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 29, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 30, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 31, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 34, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 35, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 36, 11, Engine::Instance().GetRenderer());
	for (int i = 39; i < 85; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 11, Engine::Instance().GetRenderer());
	}

	for (int i = 48; i < 58; i++)
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

	MapObjectManager::CreateMapObject(kPlate, 3, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 4, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 5, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 6, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 7, 7, Engine::Instance().GetRenderer());

	EnemyManager::CreateEnemy(archer, 4, 3, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 16, 8, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 23, 8, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 44, 8, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 53, 5, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 58, 8, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 72, 5, Engine::Instance().GetRenderer());
	//EnemyManager::CreateEnemy(archer, 1, 5, Engine::Instance().GetRenderer());

	this->m_pPortal = MapObjectManager::CreateMapObject(kPortal, 81, 6, Engine::Instance().GetRenderer());
}

void Level1::Update()
{
	if (m_pPlayer->GetDstP()->y > m_levelHight)
	{
		m_pPlayer->ChangeSoul(-m_pPlayer->m_maxSoul);
	}
	else if (COMA::AABBCheck(*this->m_pPortal->GetDstP(), *this->m_pPlayer->GetDstP()))
	{
		STMA::ChangeState(new GameState(new Level2));
	}
}

Level2::Level2()
{
	m_levelWidth = 4416;
	m_levelHight = 1300;
}

void Level2::Load(PlatformPlayer* Player)
{
	MoveManager::Init(Engine::Instance().GetWindow(), m_levelWidth, m_levelHight);

	this->m_pPlayer = Player;

	for (int i = 0; i < 20; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 11, Engine::Instance().GetRenderer());
	}
	for (int i = 22; i < 27; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 9, Engine::Instance().GetRenderer());
	}
	for (int i = 29; i < 35; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 9, Engine::Instance().GetRenderer());
	}
	for (int i = 37; i < 60; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 11, Engine::Instance().GetRenderer());
	}
	for (int i = 62; i < 85; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 11, Engine::Instance().GetRenderer());
	}


	this->m_pPortal = MapObjectManager::CreateMapObject(kPortal, 81, 6, Engine::Instance().GetRenderer());
}

void Level2::Update()
{
	if (m_pPlayer->GetDstP()->y > m_levelHight)
	{
		m_pPlayer->ChangeSoul(0);
	}
	else if (COMA::AABBCheck(*this->m_pPortal->GetDstP(), *this->m_pPlayer->GetDstP()))
	{
		STMA::ChangeState(new EndState());
	}
}
