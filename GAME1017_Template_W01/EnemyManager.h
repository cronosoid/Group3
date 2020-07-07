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

	static void Init();
	static void CreateEnemy(EnemyType type, int x, int y, SDL_Renderer* r,PlatformPlayer* hero, std::vector<MapObject*> mapObjects);
	static void DestroyInvalidEnemies();
	static std::vector<Enemies*> EnemiesVec;
private:

};