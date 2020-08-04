#include "EnemyManager.h"

#include"TextureManager.h"
#include "PlatformPlayer.h"
#include"SoundManager.h"
#include "Swordman.h"
#include "Archer.h"
#include "Boss.h"

#include <time.h>
#include <iostream>


std::vector<Enemies*> EnemyManager::EnemiesVec;
PlatformPlayer* EnemyManager::target;
Enemies* EnemyManager::m_boss;

EnemyManager::EnemyManager()
{

}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Init()
{
	TEMA::RegisterTexture("../Spritesheets/Archer.png", "Archer");
	TEMA::RegisterTexture("../Spritesheets/Pehot2.png", "Swordman");
	SOMA::Load("Aud/Arrow_hit.wav", "ArrowHit", SOUND_SFX);
	SOMA::Load("Aud/Sword_hit.wav", "SwordHit", SOUND_SFX);
}

void EnemyManager::CreateEnemy(EnemyType type, int x, int y, SDL_Renderer* r)
{	
	switch (type)
	{
	case 1:
	{
		SDL_FRect d = { x * 64.0f,y * 64.0f,192.0f,192.0f };
		Swordman* TempEnemy = new Swordman({ 0,0,64,64 }, d, r, TextureManager::GetTexture("Swordman"));
		TempEnemy->addAnimator(new Animator(TempEnemy));
		EnemiesVec.push_back(TempEnemy);
		TempEnemy = nullptr;
		std::cout << "Swordman Created!" << std::endl;
	}
	break;
	case 2:
	{
		SDL_FRect d = { x * 64.0f,y * 64.0f,128.0f,128.0f };
		Archer* TempEnemy = new Archer({ 0,0,64,64 }, d, r, TextureManager::GetTexture("Archer"));
		TempEnemy->addAnimator(new Animator(TempEnemy));
		EnemiesVec.push_back(TempEnemy);
		TempEnemy = nullptr;
		std::cout << "Archer Created!" << std::endl;
	}
	break;
	}
}

void EnemyManager::SpawnBoss(int x, int y, SDL_Renderer* r)
{
	if (!m_boss)
	{
		SDL_FRect d = { x * 64.0f,y * 64.0f,256.0f,256.0f };
		Boss* TempEnemy = new Boss(d, r); // Will be "Boss"
		TempEnemy->addAnimator(new Animator(TempEnemy));
		EnemiesVec.push_back(TempEnemy);
		m_boss = TempEnemy;
		TempEnemy = nullptr;
		std::cout << "Boss Created!" << std::endl;
	}
}

void EnemyManager::DestroyInvalidEnemies()
{
	for (auto enemy = EnemiesVec.begin(); enemy != EnemiesVec.end();)
	{
		if (not (*enemy)->getActive())
		{
			if (*enemy == m_boss)
			{
				DestroyBoss();
			}
			delete* enemy;
			enemy = EnemiesVec.erase(enemy);
		}
		else
		{
			++enemy;
		}
	}
}

void EnemyManager::DestroyBoss()
{
	m_boss = nullptr;
}
