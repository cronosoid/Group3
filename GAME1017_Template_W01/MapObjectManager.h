#pragma once
#include <vector>
#include "MapObject.h"

enum MapObjectType
{
	kPlate = 1,
	kSpike,
	kPortal,
	kUnstableBrick
};

class MapObjectManager
{
public:
	static std::vector<MapObject*> MapObjVec;
	static std::vector<SDL_FRect*> MapObjRectVec;
	static std::vector<UnstableBrick*> UnstableBrickVec;
private:

public:
	MapObjectManager();
	~MapObjectManager();
	static void Init();
	static MapObject* CreateMapObject(MapObjectType type, int x, int y, SDL_Renderer* r);
	static void CreateMoveMapObject(MapObjectType type, int x, int y, SDL_FPoint start, SDL_FPoint end, SDL_Renderer* r);
	static void Update();
	static void Render(bool CanCollide);
	static void DestroyAllMapObjects();
	static void DestroyInvalidMapObject();
private:

};