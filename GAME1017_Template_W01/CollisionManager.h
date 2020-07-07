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

class CollisionManager
{
public:
	static bool AABBCheck(const SDL_FRect& object1, const SDL_FRect& object2);

	static bool CircleCircleCheck(const SDL_FPoint object1, const SDL_FPoint object2, const double r1, const double r2 = 0.0);
	static bool CircleAABBCheck(const SDL_FPoint object1, const double r, const SDL_FRect& object2);

	static bool LinePointCheck(const SDL_FPoint object1_start, const SDL_FPoint object1_end, const SDL_FPoint object2);

	static bool PointRectCheck(const SDL_FPoint point, const SDL_FRect& object1);
	static MapObject* FindFirstObjectOnTheRay(SDL_FPoint Pos, SDL_FPoint Move, float maxDist = 9e3);
	static float SquareRectDistance(const SDL_FRect& object1, const SDL_FRect& object2);

	static void CheckMapCollision(const std::vector<MapObject*> mapObjects, Entity* obj);

	static bool CheckPortalCollision(const std::vector<MapObject*> map_objects, Entity* obj);
	
private:
	CollisionManager() {}
};

typedef CollisionManager COMA;

#endif