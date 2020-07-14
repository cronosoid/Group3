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
	totalMoveX = totalMoveY = 0;
}

void MoveManager::MoveX(float x)
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
void MoveManager::MoveY(float x)
{
	if (totalMoveY + x < 0)
	{
		x = -totalMoveY;
		totalMoveY = 0;
	}
	else if (totalMoveY + x > maxY)
	{
		totalMoveY = maxY;
		x = maxY - totalMoveY;
	}
	else
	{
		totalMoveY += x;
	}

	for (MapObject* mapObject : MapObjectManager::MapObjVec)
	{
		mapObject->GetDstP()->y -= x;
	}
	for (Enemies* enemy : EnemyManager::EnemiesVec)
	{
		enemy->GetDstP()->y -= x;
	}
	for (Projectile* projectile : PMA::Instance().GetProjectiles())
	{
		projectile->GetDstP()->y -= x;
	}
}

bool MoveManager::CheckXBoundaries(float x, float velX, float y, Entity* player)
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
bool MoveManager::checkYBoundaries(float Y, float velY, float x, Entity* player)
{
	std::cout << totalMoveY << "\n";
	int upBorder = windowY - windowY * MAPSCROLLINGDISTANCE;
	int downBorder = windowY * MAPSCROLLINGyDISTANCE;
	if ((Y + velY < upBorder and velY < 0 and totalMoveY > 0) or (Y + velY > downBorder and velY > 0 and totalMoveY < maxY))
	{
		return true;
	}
	return false;
}