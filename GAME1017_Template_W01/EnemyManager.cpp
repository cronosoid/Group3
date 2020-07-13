#include "EnemyManager.h"
#include"TextureManager.h"
#include <time.h>
#include <cstdlib>
#include <iostream>
#include "PlatformPlayer.h"
#include"SoundManager.h"

std::vector<Enemies*> EnemyManager::EnemiesVec;
PlatformPlayer* EnemyManager::target;

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
		SDL_FRect d = { x * 64.0f,y * 64.0f,128.0f,128.0f };
		std::cout << "ENMA:" << d.x << ", " << d.y << ", " << d.w << ", " << d.h << "\n";
		Swordman* TempEnemy = new Swordman({ 0,0,640,640 }, d, r, TextureManager::GetTexture("Swordman"));
		TempEnemy->addAnimator(new Animator(TempEnemy));
		EnemiesVec.push_back(TempEnemy);
		TempEnemy = nullptr;
		std::cout << "Swordman Created!" << std::endl;
	}
	break;
	case 2:
	{
		SDL_FRect d = { x * 64.0f,y * 64.0f,128.0f,128.0f };
		Archer* TempEnemy = new Archer({ 0,0,640,640 }, d, r, TextureManager::GetTexture("Archer"));
		TempEnemy->addAnimator(new Animator(TempEnemy));
		EnemiesVec.push_back(TempEnemy);
		TempEnemy = nullptr;
		std::cout << "Archer Created!" << std::endl;
	}
	break;
	}
}

void EnemyManager::DestroyInvalidEnemies()
{
	for (auto enemy = EnemiesVec.begin(); enemy != EnemiesVec.end();)
	{
		if (not (*enemy)->getActive())
		{
			delete* enemy;
			enemy = EnemiesVec.erase(enemy);
		}
		else
		{
			++enemy;
		}
	}
}
