#pragma once
#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include <iostream>
#include <SDL.h>
#include "MathManager.h"
#include "Sprite.h"
#include "Entity.h"
#include "MapObjectManager.h"

#include <vector>
#include "MapObject.h"
#include"PlatformPlayer.h"
#include "Enemies.h"

class CollisionManager
{
public:
	static bool AABBCheck(const SDL_FRect& object1, const SDL_FRect& object2);

	static bool CircleCircleCheck(const SDL_FPoint object1, const SDL_FPoint object2, const double r1, const double r2 = 0.0);
	static bool CircleAABBCheck(const SDL_FPoint object1, const double r, const SDL_FRect& object2);

	static bool LinePointCheck(const SDL_FPoint object1_start, const SDL_FPoint object1_end, const SDL_FPoint object2);

	static void CheckPlayerMapDamage(const std::vector<MapObject*> mapObject, PlatformPlayer* obj);
	static void CheckEnemyMapDamage(const std::vector<MapObject*> mapObject, Enemies* obj);
	static bool PointRectCheck(const SDL_FPoint point, const SDL_FRect& object1);
	static bool LineLineCheck(SDL_FPoint line1_start, SDL_FPoint line1_end, SDL_FPoint line2_start, SDL_FPoint line2_end);
	static bool LineRectCheck(const SDL_FPoint& line1_start, const SDL_FPoint& line1_end, const SDL_FRect* box);
	static MapObject* FindFirstObjectOnTheRay(SDL_FPoint Pos, SDL_FPoint Move, float maxDist = 9e3);
	static bool HaveLOS(Entity* from, Entity* to);
	static float SquareRectDistance(const SDL_FRect& object1, const SDL_FRect& object2);
	static void CheckMapCollision(const std::vector<MapObject*> mapObjects, Entity* obj);
	static void CheckUnstableBrickCollision(const std::vector<UnstableBrick*> bricks, Entity* obj);
	
private:
	CollisionManager() {}
};

typedef CollisionManager COMA;

#endif