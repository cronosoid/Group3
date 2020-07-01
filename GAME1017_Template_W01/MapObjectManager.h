#pragma once
#include <vector>
#include "MapObject.h"

enum MapObjectType
{
	kPlate = 1,
	kSpike
};

class MapObjectManager
{
public:
	static std::vector<MapObject*> MapObjVec;
	static std::vector<SDL_FRect*> MapObjRectVec;
private:

public:
	MapObjectManager();
	~MapObjectManager();
	static void Init();
	static void CreateMapObject(MapObjectType type, SDL_FRect dst_Rect, SDL_Renderer* r);
	static void Update();
	static void Render(bool CanCollide);
	static void DestroyMapObjects();
private:

};