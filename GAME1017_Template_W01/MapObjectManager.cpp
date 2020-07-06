#include "MapObjectManager.h"

#include <iostream>

#include"TextureManager.h"

std::vector<MapObject*> MapObjectManager::MapObjVec;
std::vector<SDL_FRect*> MapObjectManager::MapObjRectVec;

MapObjectManager::MapObjectManager()
{

}

MapObjectManager::~MapObjectManager()
{

}

void MapObjectManager::Init()
{
	TextureManager::RegisterTexture("Img/brick.png", "plate");
	TextureManager::RegisterTexture("Img/spike.png", "spike");
	TextureManager::RegisterTexture("Img/portal.png", "portal");
}

void MapObjectManager::CreateMapObject(MapObjectType type, SDL_FRect dst_Rect, SDL_Renderer* r)
{ 
	switch (type)
	{
	case kPlate:
	{
		Plate* TempMapObj = new Plate({ 0,0,64,64 }, dst_Rect, r, TextureManager::GetTexture("plate"));
		MapObjVec.push_back(TempMapObj);
		SDL_FRect* TempFRect = TempMapObj->getFRect();
		MapObjRectVec.push_back(TempFRect);
		TempMapObj = nullptr;
		TempFRect = nullptr;
		break;
	}
	case kSpike:
	{
		std::cout << "spike created" << std::endl;
		Spike* TempMapObj = new Spike({ 0,0,64,64 }, dst_Rect, r, TextureManager::GetTexture("spike"));
		MapObjVec.push_back(TempMapObj);
		SDL_FRect* TempFRect = TempMapObj->getFRect();
		MapObjRectVec.push_back(TempFRect);
		TempMapObj = nullptr;
		TempFRect = nullptr;
		break;
	}

	case kPortal:
	{
		std::cout << "portal created" << std::endl;
		Portal* TempMapObj = new Portal({ 0,0,320,320 }, dst_Rect, r, TextureManager::GetTexture("portal"));
		std::cout << TempMapObj->getType() << std::endl;
		MapObjVec.push_back(TempMapObj);
		SDL_FRect* TempFRect = TempMapObj->getFRect();
		MapObjRectVec.push_back(TempFRect);
		TempMapObj = nullptr;
		TempFRect = nullptr;
	}
		
	default:
		break;
	}
}

void MapObjectManager::Update()
{
	for (int i = 0; i < (int)MapObjVec.size(); i++)
	{
		MapObjVec[i]->Update();
	}
}

void MapObjectManager::Render(bool collision)
{
	for (MapObject* mapObject : MapObjVec)
	{
		if (mapObject->getCollision() == collision)
		{
			mapObject->Render();
		}
	}
}

void MapObjectManager::DestroyMapObjects()
{
	for (auto mapobj = MapObjVec.begin(); mapobj != MapObjVec.end(); )
	{
		delete* mapobj;
		mapobj = MapObjVec.erase(mapobj);
		++mapobj;
	}
}

