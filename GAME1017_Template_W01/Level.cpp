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
	m_currentLevel = 1;
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

	EnemyManager::CreateEnemy(swordman, 4, 3, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 16, 8, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 23, 8, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 44, 8, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 53, 5, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 58, 8, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 72, 5, Engine::Instance().GetRenderer());


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
		//STMA::ChangeState(new GameState(new Level2));
		STMA::ChangeState(new CongratulationState);
	}
}

Level2::Level2()
{
	m_levelWidth = 5504;
	m_levelHight = 1300;
	m_currentLevel = 2;
}

void Level2::Load(PlatformPlayer* Player)
{
	MoveManager::Init(Engine::Instance().GetWindow(), m_levelWidth, m_levelHight);

	this->m_pPlayer = Player;

	for (int i = 0; i < 7; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 11, Engine::Instance().GetRenderer());
	}
	MapObjectManager::CreateMapObject(kPlate, 10, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 11, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 12, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 15, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 16, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 17, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 18, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 19, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 20, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 21, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 22, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 23, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 24, 11, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 25, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 26, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 27, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 28, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 29, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 30, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 31, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 32, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 33, 9, Engine::Instance().GetRenderer());
	for (int i = 34; i < 66; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 11, Engine::Instance().GetRenderer());
	}
	MapObjectManager::CreateMapObject(kPlate, 25, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 53, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 54, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 55, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 56, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 57, 7, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 66, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 66, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 69, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 70, 9, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 71, 9, Engine::Instance().GetRenderer());
	for (int i = 72; i < 102; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 11, Engine::Instance().GetRenderer());
	}
	MapObjectManager::CreateMapObject(kPlate, 85, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 86, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 87, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 89, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 90, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 91, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 93, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 94, 8, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kPlate, 95, 8, Engine::Instance().GetRenderer());

	MapObjectManager::CreateMapObject(kSpike, 39, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 42, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 43, 10, Engine::Instance().GetRenderer());
	//MapObjectManager::CreateMapObject(kSpike, 44, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 47, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 55, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 56, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 57, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 76, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 77, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 80, 10, Engine::Instance().GetRenderer());
	MapObjectManager::CreateMapObject(kSpike, 81, 10, Engine::Instance().GetRenderer());

	this->m_pPortal = MapObjectManager::CreateMapObject(kPortal, 98, 6, Engine::Instance().GetRenderer());

	EnemyManager::CreateEnemy(swordman, 11, 9, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 29, 5, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 32, 7, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 36, 9, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 52, 7, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 55, 5, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(swordman, 70, 7, Engine::Instance().GetRenderer());

	EnemyManager::CreateEnemy(archer, 25, 7, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(archer, 48, 9, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(archer, 61, 9, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(archer, 85, 9, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(archer, 89, 9, Engine::Instance().GetRenderer());
	EnemyManager::CreateEnemy(archer, 94, 9, Engine::Instance().GetRenderer());

	std::cout << "Portal: " << this->m_pPortal->GetDstP()->x / 64 << " " << this->m_pPortal->GetDstP()->y / 64 << std::endl;
}

void Level2::Update()
{
	if (m_pPlayer->GetDstP()->y > m_levelHight)
	{
		m_pPlayer->ChangeSoul(-m_pPlayer->m_maxSoul);
	}
	else if (COMA::AABBCheck(*this->m_pPortal->GetDstP(), *this->m_pPlayer->GetDstP()))
	{
		//STMA::ChangeState(new EndState());
		STMA::ChangeState(new CongratulationState);
	}
}

Level3::Level3()
{
	m_levelWidth = 5504;
	m_levelHight = 1300;
	m_currentLevel = 3;
}

void Level3::Load(PlatformPlayer* Player)
{
	MoveManager::Init(Engine::Instance().GetWindow(), m_levelWidth, m_levelHight);

	this->m_pPlayer = Player;

	for (int i = 0; i < 7; i++)
	{
		MapObjectManager::CreateMapObject(kPlate, i, 11, Engine::Instance().GetRenderer());
	}
	this->m_pPortal = MapObjectManager::CreateMapObject(kPortal, 5, 6, Engine::Instance().GetRenderer());
}

void Level3::Update()
{
	if (m_pPlayer->GetDstP()->y > m_levelHight)
	{
		m_pPlayer->ChangeSoul(-m_pPlayer->m_maxSoul);
	}
	else if (COMA::AABBCheck(*this->m_pPortal->GetDstP(), *this->m_pPlayer->GetDstP()))
	{
		//STMA::ChangeState(new EndState());
		STMA::ChangeState(new CongratulationState);
	}
}

