#include "CollisionManager.h"
#include "DebugManager.h"
#include "StateManager.h"
#include "MoveManager.h"
#include "SoundManager.h"
#include "TextureManager.h"

bool CollisionManager::AABBCheck(const SDL_FRect& object1, const SDL_FRect& object2)
{
	return (object1.x < object2.x + object2.w and
		object1.x + object1.w > object2.x and
		object1.y < object2.y + object2.h and
		object1.y + object1.h > object2.y);
}

bool CollisionManager::CircleCircleCheck(const SDL_FPoint object1, const SDL_FPoint object2, const double r1, const double r2)
{
	return (MAMA::Distance((double)object1.x, (double)object2.x, (double)object1.y, (double)object2.y) < (r1+r2));
}

bool CollisionManager::CircleAABBCheck(const SDL_FPoint object1, const double r, const SDL_FRect& object2)
{
	double x1 = (double)object1.x;
	double y1 = (double)object1.y;
	if (object1.x < object2.x) // Circle center, rect left edge.
		x1 = (double)object2.x;
	else if (object1.x > object2.x + object2.w)
		x1 = (double)object2.x + (double)object2.w;
	if (object1.y < object2.y)
		y1 = (double)object2.y;
	else if (object1.y > object2.y + object2.h)
		y1 = (double)object2.y + (double)object2.h;

	return CircleCircleCheck({(float)x1, (float)y1}, {(float)object1.x, (float)object1.y}, r);
}

bool CollisionManager::LinePointCheck(const SDL_FPoint object1_start, const SDL_FPoint object1_end, const SDL_FPoint object2)
{
	double distToStart = MAMA::Distance((double)object1_start.x, (double)object2.x, (double)object1_start.y, (double)object2.y);
	double distToEnd = MAMA::Distance((double)object1_end.x, (double)object2.x, (double)object1_end.y, (double)object2.y);

	double lineLength = MAMA::Distance((double)object1_start.x, (double)object1_end.x, (double)object1_start.y, (double)object1_end.y);

	double buffer = 0.2; // Extra distance since line thickness is one pixel.
	
	if (distToStart + distToEnd <= lineLength + buffer)
		return true;
	return false;
}

bool CollisionManager::PointRectCheck(const SDL_FPoint point, const SDL_FRect& object1)
{
	return (point.x <= object1.x + object1.w and
		point.x >= object1.x and
		point.y <= object1.y + object1.h and
		point.y >= object1.y);
}

MapObject* CollisionManager::FindFirstObjectOnTheRay(SDL_FPoint Pos, SDL_FPoint Move, float maxDist)
{
	float len = sqrt(Move.x * Move.x + Move.y * Move.y);
	float dx = Move.x / len;
	float dy = Move.y / len;

	float dist = 0;
	bool distancing = false;
	if (maxDist < 9e3)
	{
		maxDist *= maxDist;
		distancing = true;
	}
	
	SDL_FPoint curPos = { Pos.x, Pos.y };
	while (curPos.x > -MOMA::GetTotalMove().x and curPos.x < MOMA::GetMaxX() and curPos.y > -MOMA::GetMaxY() and curPos.y < MOMA::GetWindowY() and dist < maxDist)
	{
		curPos.x += dx;
		curPos.y += dy;
		if (distancing)
		{
			dist += dx * dx + dy * dy;
		}
		for (MapObject* obj : MapObjectManager::MapObjVec)
		{
			if (obj->getCanCollide() and COMA::PointRectCheck(curPos, *obj->GetDstP()))
			{
				return obj;
			}
		}
	}
	return nullptr;
}

float CollisionManager::SquareRectDistance(const SDL_FRect& object1, const SDL_FRect& object2)
{
	float x1 = object1.x + object1.w / 2;
	float x2 = object2.x + object2.w / 2;
	float y1 = object1.y + object1.h / 2;
	float y2 = object2.y + object2.h / 2;
	return (pow(x1 - x2, 2.0f) + pow(y1 - y2, 2.0f));
}

void CollisionManager::CheckMapCollision(const std::vector<MapObject*> mapObjects, Entity* obj)
{
	MapObject* oldFloor = obj->GetFloor();
	obj->SetGrounded(false);
	for (MapObject* mapObject : mapObjects) // For each platform.
	{
		SDL_FRect* mapObjectRect = mapObject->GetDstP();
		if (mapObject->getCanCollide() and COMA::AABBCheck(*obj->GetDstP(), *mapObjectRect))
		{
			if (obj->GetDstP()->y + obj->GetDstP()->h - (float)obj->GetVelY() <= mapObjectRect->y)
			{ // Colliding top side of platform.
				MapObject* newFloor = oldFloor;
				if (oldFloor == nullptr
					or SquareRectDistance(*oldFloor->GetDstP(), *obj->GetDstP()) > SquareRectDistance(*mapObjectRect, *obj->GetDstP()))
				{
					newFloor = mapObject;
				}
				obj->SetGrounded(true, newFloor);
				obj->StopY();
				obj->SetY(mapObjectRect->y - obj->GetDstP()->h - 1);
			}
			else if (obj->GetDstP()->y - (float)obj->GetVelY() >= mapObjectRect->y + mapObjectRect->h)
			{ // Colliding bottom side of platform.
				obj->StopY();
				obj->SetY(mapObjectRect->y + mapObjectRect->h);
			}
			else if (obj->GetDstP()->x + obj->GetDstP()->w - (float)obj->GetVelX() <= mapObjectRect->x)
			{ // Collision from left.
				if ((obj->GetDstP()->y + obj->GetDstP()->h - (float)obj->GetVelY() > mapObjectRect->y) or not obj->IsGrounded())
				{
					obj->StopX();
					obj->SetX(mapObjectRect->x - obj->GetDstP()->w);
				}
			}
			else if (obj->GetDstP()->x - (float)obj->GetVelX() >= mapObjectRect->x + mapObjectRect->w)
			{ // Colliding right side of platform.
				if ((obj->GetDstP()->y + obj->GetDstP()->h - (float)obj->GetVelY() > mapObjectRect->y) or not obj->IsGrounded())
				{
					obj->StopX();
					obj->SetX(mapObjectRect->x + mapObjectRect->w);
				}
			}
		}
	}
}

void CollisionManager::CheckPlayerMapDamage(const std::vector<MapObject*> mapObject, PlatformPlayer* obj)
{
	for (auto mapObject : mapObject) // For each platform.
	{
		SDL_FRect* temp = mapObject->GetDstP();
		if (COMA::AABBCheck(*obj->GetDstP(), *temp) && mapObject->getIsHurt() == true && obj->GetLastAttackedTime()<=0)
		{
				obj->ChangeSoul(-mapObject->getDamage());
				obj->SetLastAttackedTime();
				std::cout << "Health: " << obj->GetSoul() << std::endl;
				SOMA::PlaySound("Spike_hit");
				break;
		}
	}
}

void CollisionManager::CheckEnemyMapDamage(const std::vector<MapObject*> mapObject, Enemies* obj)
{
	for (auto mapObject : mapObject) // For each platform.
	{
		SDL_FRect* temp = mapObject->GetDstP();
		if (COMA::AABBCheck(*obj->GetDstP(), *temp) && mapObject->getIsHurt() == true)
		{
			obj->getDamage(mapObject->getDamage());
			SOMA::PlaySound("Spike_hit");
			break;
		}
	}
}