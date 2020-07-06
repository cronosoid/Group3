#include "MoveManager.h"
#include "EnemyManager.h"
#include "ProjectileManager.h"

int MoveManager::maxX, MoveManager::maxY;
int MoveManager::windowX, MoveManager::windowY;
int MoveManager::border;
int MoveManager::totalMoveX, MoveManager::totalMoveY;

MoveManager::MoveManager()
{
}

MoveManager::~MoveManager()
{
}

void MoveManager::Init(SDL_Window* window, int x, int y)
{
	SDL_GetWindowSize(window, &windowX, &windowY);
	maxX = x;
	maxY = y;
	border = windowX * 0.1;
}

void MoveManager::moveX(float x)
{
	if (totalMoveX + x < 0)
	{
		x = -totalMoveX;
		totalMoveX = 0;
	}
	else if (totalMoveX + x > maxX)
	{
		totalMoveX = maxX;
		x = maxX - totalMoveX;
	}
	else
	{
		totalMoveX += x;
	}

	for (MapObject* mapObject : MapObjectManager::MapObjVec)
	{
		mapObject->GetDstP()->x -= x;
		if(mapObject->getCanMove())
		{
			mapObject->moveStart(-x, 0.0f);
			mapObject->moveEnd(-x, 0.0f);
		}
	}
	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		enemy->GetDstP()->x -= x;
	}
	for (Projectile* projectile : PMA::Instance().GetProjectiles())
	{
		projectile->GetDstP()->x -= x;
	}
}

bool MoveManager::checkBoundaries(float x, float velX, float y, Entity* player)
{
	//std::cout << totalMoveX << "\n";
	int rightBorder = windowX - windowX * MAPSCROLLINGDISTANCE;
	int leftBorder = windowX * MAPSCROLLINGDISTANCE;
	if ((x + velX < leftBorder and velX < 0 and totalMoveX > 0) or (x + velX > rightBorder and velX > 0 and totalMoveX < maxX))
	{
		return true;
	}
	return false;
}
