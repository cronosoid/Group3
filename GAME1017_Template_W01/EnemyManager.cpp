#include "EnemyManager.h"
#include"TextureManager.h"
#include <time.h>
#include <cstdlib>
#include <iostream>

std::vector<Enemies*> EnemyManager::EnemiesVec;

EnemyManager::EnemyManager()
{

}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::CreateEnemy(EnemyType type, SDL_FRect d, SDL_Renderer* r)
{
	switch (type)
	{
	case 1:
	{
		Swordman* TempEnemy = new Swordman({ 0,0,640,640 }, d, r, TextureManager::GetTexture("Swordman"));
		EnemiesVec.push_back(TempEnemy);
		TempEnemy = nullptr;
		std::cout << "Swordman Created!" << std::endl;
	}
	break;
	case 2:
	{
		Archer* TempEnemy = new Archer({ 0,0,640,640 }, d, r, TextureManager::GetTexture("Archer"));
		EnemiesVec.push_back(TempEnemy);
		TempEnemy = nullptr;
		std::cout << "Archer Created!" << std::endl;
	}
	break;
	}
}

void EnemyManager::DestroyInvalidEnemies()
{
	for (int i = 0; i < (int)EnemiesVec.size(); i++)
	{
		if (EnemiesVec[i]->getActive() == false)
		{
			std::cout << EnemiesVec[i]->getType();
			SDL_DestroyTexture(EnemiesVec[i]->getTex());
			delete EnemiesVec[i];
			EnemiesVec[i] = nullptr;
			std::cout << " has been destroyed" << std::endl;
		}
	}
	if (!EnemiesVec.empty())
	{
		EnemiesVec.erase(remove(EnemiesVec.begin(), EnemiesVec.end(), nullptr), EnemiesVec.end());
	}
}
