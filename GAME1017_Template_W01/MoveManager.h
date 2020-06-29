#pragma once
#ifndef _MOVEMANAGER_H_
#define _MOVEMANAGER_H_

#include "MapObjectManager.h"
#include "Entity.h"

#include <vector>

const float MAPSCROLLINGDISTANCE = 0.3f;

class MoveManager
{
public:
	MoveManager();
	~MoveManager();

	static void Init(SDL_Window* window, int maxX, int maxY);
	static void moveX(float x);
	static int getMaxX() { return maxX; }
	static int getMaxY() { return maxY; }
	static bool checkBoundaries(float x, float velX, float y, Entity* player);
	static SDL_FRect getTotalMove() { return {(float)totalMoveX, (float)totalMoveY, 0, 0}; }
	static int getRightBorder() { return maxX; }

private:
	static int maxX, maxY;
	static int windowX, windowY;
	static int border;
	static int totalMoveX, totalMoveY;
};

typedef MoveManager MOMA;
#endif