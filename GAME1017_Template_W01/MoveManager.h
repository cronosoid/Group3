#pragma once
#ifndef _MOVEMANAGER_H_
#define _MOVEMANAGER_H_

#include "MapObjectManager.h"
#include "Entity.h"

#include <vector>

const float MAPSCROLLINGDISTANCE = 0.3f;
const float MAPSCROLLINGYUPDISTANCE = 0.3f;
const float MAPSCROLLINGYDOWNDISTANCE = 0.3f;

class MoveManager
{
public:
	MoveManager();
	~MoveManager();

	static void Init(SDL_Window* window, int maxX, int maxY);
	static void MoveX(float x);
	static void MoveY(float x);
	static int GetMaxX() { return maxX; }
	static int GetMaxY() { return maxY; }
	static bool CheckXBoundaries(float x, float velX, Entity* player);
	static bool checkYBoundaries(float y, float velY, Entity* player);
	static SDL_FRect GetTotalMove() { return {(float)totalMoveX, (float)totalMoveY, 0, 0}; }
	static int GetRightBorder() { return maxX; }
	static int GetWindowY() { return windowY; }
	static int GetWindowX() { return windowX; }
	static void SetY(int y);
	
private:
	static int maxX, maxY;
	static int windowX, windowY;
	static int border;
	static int totalMoveX, totalMoveY;
};

typedef MoveManager MOMA;
#endif