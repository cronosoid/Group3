#pragma once
#include <vector>
#include "Animator.h"
#include "Archer.h"
#include "Swordman.h"

enum EnemyType
{
	swordman = 1,
	archer
};

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	static void CreateEnemy(EnemyType type, SDL_FRect d, SDL_Renderer* r);
	static void MoveEnemy(EnemyType type, SDL_FRect d, SDL_Renderer* r);
	static void DestroyInvalidEnemies();
	static std::vector<Enemies*> EnemiesVec;
private:

};