#include "MapObjectManager.h"

#include <iostream>

#include"TextureManager.h"

std::vector<MapObject*> MapObjectManager::MapObjVec;
std::vector<SDL_FRect*> MapObjectManager::MapObjRectVec;
std::vector<UnstableBrick*> MapObjectManager::UnstableBrickVec;

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
	TextureManager::RegisterTexture("Img/BrokenBrick.png", "unstableBrick");
}

MapObject* MapObjectManager::CreateMapObject(MapObjectType type, int x, int y, SDL_Renderer* r)
{
	switch (type)
	{
	case kPlate:
	{
		Plate* TempMapObj = new Plate({ 0,0,64,64 }, {x*64.0f,y*64.0f,64.0f,64.0f}, r, TextureManager::GetTexture("plate"));
		MapObjVec.push_back(TempMapObj);
		SDL_FRect* TempFRect = TempMapObj->GetDstP();
		MapObjRectVec.push_back(TempFRect);
		TempMapObj = nullptr;
		TempFRect = nullptr;
		break;
	}
	case kSpike:
	{
		Spike* TempMapObj = new Spike({ 0,0,64,64 }, { x * 64.0f,y * 64.0f,64.0f,64.0f }, r, TextureManager::GetTexture("spike"));
		MapObjVec.push_back(TempMapObj);
		SDL_FRect* TempFRect = TempMapObj->GetDstP();
		MapObjRectVec.push_back(TempFRect);
		TempMapObj = nullptr;
		TempFRect = nullptr;
		break;
	}
   case kPortal:
	{
		std::cout << "portal created" << std::endl;
		Portal* TempMapObj = new Portal({ 0,0,320,320 }, {x*64.0f,y*64.0f,320.0f,320.0f}, r, TextureManager::GetTexture("portal"));
		//std::cout << TempMapObj->getType() << std::endl;
		MapObjVec.push_back(TempMapObj);
		SDL_FRect* TempFRect = TempMapObj->GetDstP();
		MapObjRectVec.push_back(TempFRect);
		TempMapObj = nullptr;
		TempFRect = nullptr;
	}
   case kUnstableBrick:
	{
	   UnstableBrick* TempMapObj = new UnstableBrick({ 0,0,64,64 }, { x * 64.0f,y * 64.0f,64.0f,64.0f }, r, TextureManager::GetTexture("unstableBrick"));
	   MapObjVec.push_back(TempMapObj);
	   UnstableBrickVec.push_back(TempMapObj);
	   SDL_FRect* TempFRect = TempMapObj->GetDstP();
	   MapObjRectVec.push_back(TempFRect);
	   TempMapObj = nullptr;
	   TempFRect = nullptr;
	   break;
	}
	default:
		break;
	}
	return MapObjVec.back();
}

void MapObjectManager::CreateMoveMapObject(MapObjectType type, int x, int y, SDL_FPoint start, SDL_FPoint end, SDL_Renderer* r)
{
	switch (type)
	{
	case kPlate:
	{
		Plate* TempMapObj = new Plate({ 0,0,64,64 }, { x * 64.0f,y * 64.0f,64.0f,64.0f }, r, TextureManager::GetTexture("plate"),start,end);
		MapObjVec.push_back(TempMapObj);
		SDL_FRect* TempFRect = TempMapObj->GetDstP();
		MapObjRectVec.push_back(TempFRect);
		TempMapObj = nullptr;
		TempFRect = nullptr;
		break;
	}
	case kSpike:
	{
		Spike* TempMapObj = new Spike({ 0,0,64,64 }, { x * 64.0f,y * 64.0f,64.0f,64.0f }, r, TextureManager::GetTexture("spike"),start,end);
		//std::cout << "Moving Spike Created" << std::endl;
		MapObjVec.push_back(TempMapObj);
		SDL_FRect* TempFRect = TempMapObj->GetDstP();
		MapObjRectVec.push_back(TempFRect);
		TempMapObj = nullptr;
		TempFRect = nullptr;
		break;
	}
	case kUnstableBrick:
	{
		UnstableBrick* TempMapObj = new UnstableBrick({ 0,0,64,64 }, { x * 64.0f,y * 64.0f,64.0f,64.0f }, r, TextureManager::GetTexture("unstableBrick"),start,end);
		MapObjVec.push_back(TempMapObj);
		UnstableBrickVec.push_back(TempMapObj);
		SDL_FRect* TempFRect = TempMapObj->GetDstP();
		MapObjRectVec.push_back(TempFRect);
		TempMapObj = nullptr;
		TempFRect = nullptr;
		break;
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


void MapObjectManager::Render(bool CanCollide)
{
	for (MapObject* mapObject : MapObjVec)
	{
		if(mapObject->getCanCollide()!=CanCollide)
		{
			mapObject->Render();
		}
	}
}

void MapObjectManager::DestroyAllMapObjects()
{
	for (auto mapobj = MapObjVec.begin(); mapobj != MapObjVec.end(); )
	{
		delete* mapobj;
		mapobj = MapObjVec.erase(mapobj);
		++mapobj;
	}
}

void MapObjectManager::DestroyInvalidMapObject()
{
	for (auto mapobj = MapObjVec.begin(); mapobj != MapObjVec.end(); )
	{
		if(not (*mapobj)->getActive())
		{
			delete* mapobj;
			mapobj = MapObjVec.erase(mapobj);
		}		
		++mapobj;
	}
}
