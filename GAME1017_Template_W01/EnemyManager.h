#pragma once
#include <vector>

#include "Enemies.h"
#include "PlatformPlayer.h"

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
	static void CreateEnemy(EnemyType type, int x, int y, SDL_Renderer* r);
	static void SpawnBoss(int x, int y, SDL_Renderer* r);
	static void DestroyInvalidEnemies();
	static std::vector<Enemies*> EnemiesVec;
	static PlatformPlayer* GetTarget() { return target; }
	static void SetTarget(PlatformPlayer* newTarget = nullptr) { target = newTarget; }
	static Enemies* GetBoss() { return m_boss; }
	static void DestroyBoss();
	static bool GetBossActive() { return (m_boss != nullptr); }
	static float GetBossLocX() { return m_boss->GetDstP()->x; }
private:
	static PlatformPlayer* target;
	static Enemies* m_boss;
};