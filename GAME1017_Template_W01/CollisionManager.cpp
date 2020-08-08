#include "CollisionManager.h"
#include "DebugManager.h"
#include "StateManager.h"
#include "MoveManager.h"
#include "TextureManager.h"
#include "SoundManager.h"

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

bool CollisionManager::LineLineCheck(SDL_FPoint line1_start, SDL_FPoint line1_end, SDL_FPoint line2_start, SDL_FPoint line2_end)
{
	const auto x1 = line1_start.x;
	const auto x2 = line1_end.x;
	const auto x3 = line2_start.x;
	const auto x4 = line2_end.x;
	const auto y1 = line1_start.y;
	const auto y2 = line1_end.y;
	const auto y3 = line2_start.y;
	const auto y4 = line2_end.y;

	// calculate the distance to intersection point
	const auto uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	const auto uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		return true;
	}

	return false;
}

bool CollisionManager::LineRectCheck(const SDL_FPoint& line1_start, const SDL_FPoint& line1_end, const SDL_FRect* box)
{
	const auto x1 = line1_start.x;
	const auto x2 = line1_end.x;
	const auto y1 = line1_start.y;
	const auto y2 = line1_end.y;
	const auto rx = box->x;
	const auto ry = box->y;
	const auto rw = box->w;
	const auto rh = box->h;

	// check if the line has hit any of the rectangle's sides
	// uses the Line/Line function below
	const bool left = LineLineCheck({ x1, y1 }, { x2, y2 }, { rx, ry }, { rx, ry + rh });
	const bool right = LineLineCheck({ x1, y1 }, { x2, y2 }, { rx + rw, ry }, { rx + rw, ry + rh });
	const bool top = LineLineCheck({ x1, y1 }, { x2, y2 }, { rx, ry }, { rx + rw, ry });
	const bool bottom = LineLineCheck({ x1, y1 }, { x2, y2 }, { rx, ry + rh }, { rx + rw, ry + rh });

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom) {
		return true;
	}

	return false;
}

bool CollisionManager::HaveLOS(Entity* from, Entity* to)
{
	for (MapObject* obj : MapObjectManager::MapObjVec)
	{
		const SDL_FRect* box = obj->GetDstP();

		if (obj->getCanCollide() and LineRectCheck(from->GetCenter(), to->GetCenter(), box))
		{
			return false;
		}
	}

	return true;
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
			if ((obj->getCanCollide() or obj->getIsHurt()) and COMA::PointRectCheck(curPos, *obj->GetDstP()))
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
		SDL_FRect* entityRect = obj->GetBody();
		if (mapObject->getCanCollide() and COMA::AABBCheck(*entityRect, *mapObjectRect))
		{
			if (entityRect->x + entityRect->w - (float)obj->GetVelX() <= mapObjectRect->x + mapObjectRect->w * 0.2)
			{ // Colliding from right
				if ((entityRect->y + entityRect->h - (float)obj->GetVelY() > mapObjectRect->y))
				{
					obj->StopX();
					obj->SetX(mapObjectRect->x - entityRect->w);
				}
			}
			else if (entityRect->x - (float)obj->GetVelX() >= mapObjectRect->x + mapObjectRect->w * 0.8)
			{ // Collision from left
				if ((entityRect->y + entityRect->h - (float)obj->GetVelY() > mapObjectRect->y))
				{
					obj->StopX();
					obj->SetX(mapObjectRect->x + mapObjectRect->w);
				}
			}
		}
	}
	for (MapObject* mapObject : mapObjects) // For each platform.
	{
		SDL_FRect* mapObjectRect = mapObject->GetDstP();
		SDL_FRect* entityRect = obj->GetBody();
		if (mapObject->getCanCollide() and COMA::AABBCheck(*entityRect, *mapObjectRect))
		{
			if (entityRect->y + entityRect->h - (float)obj->GetVelY() <= mapObjectRect->y + mapObjectRect->h * 0.2)
			{ // Colliding top side of platform.
				MapObject* newFloor = oldFloor;
				if (oldFloor == nullptr
					or SquareRectDistance(*oldFloor->GetDstP(), *entityRect) > SquareRectDistance(*mapObjectRect, *entityRect))
				{
					newFloor = mapObject;
				}
				obj->SetGrounded(true, newFloor);
				obj->StopY();
				obj->SetY(mapObjectRect->y - entityRect->h - 1);
			}
			else if (entityRect->y - (float)obj->GetVelY() >= mapObjectRect->y + mapObjectRect->h)
			{ // Colliding bottom side of platform.
				obj->StopY();
				obj->SetY(mapObjectRect->y + mapObjectRect->h);
			}
			
		}
	}	
}

void CollisionManager::CheckUnstableBrickCollision(const std::vector<UnstableBrick*> bricks, Entity* obj)
{
	for (auto mapObject : bricks)
	{
		SDL_FRect* mapObjectRect = mapObject->GetDstP();
		SDL_FRect* body = obj->GetBody();
		//std::cout << mapObject->getCanCollide() << " " << COMA::AABBCheck(*obj->GetDstP(), *mapObjectRect) << std::endl;
		if (mapObject->getCanCollide() && COMA::AABBCheck(*body, *mapObjectRect) )
		{
			if (body->y + body->h - (float)obj->GetVelY() <= mapObjectRect->y + mapObjectRect->h * 0.2)
			{ // Colliding top side of platform.
				if(mapObject->getStatus()==1)
				{
					mapObject->changeStatus(2);//change status into breaking
				}				
			}			
		}
	}
}

void CollisionManager::CheckProMapCollision(const std::vector<Projectile*> proVec, const std::vector<MapObject*> mapVec)
{
	for(auto pro:proVec)
		for(auto map:mapVec)
		{
			if(map->getCanCollide()==true && AABBCheck(*(pro->GetDstP()),*(map->GetDstP())))
			{
				pro->setIsActive(false);
			}
		}
}

void CollisionManager::CheckPlayerMapDamage(const std::vector<MapObject*> mapObject, PlatformPlayer* obj)
{
	for (auto mapObject : mapObject) // For each platform.
	{
		SDL_FRect* temp = mapObject->GetDstP();
		SDL_FRect* body = obj->GetBody();
		if (COMA::AABBCheck(*body, *temp) && mapObject->getIsHurt() == true && obj->GetLastAttackedTime()<=0)
		{
			if (body->x < (mapObject->GetDstP()->x + mapObject->GetDstP()->w) && body->x > mapObject->GetDstP()->x)
			{
				obj->setHitDir(1);
				SOMA::PlaySound("spikeHit", 0 , 3);
				//set hit direction as left
				//std::cout << "Kaben: " << body->x << " Spike: " << mapObject->GetDstP()->x << std::endl;
				//std::cout << "set map object hit direction as left" << std::endl;
			}
			else if ((body->x + body->w) > mapObject->GetDstP()->x && body->x < mapObject->GetDstP()->x)
			{
				obj->setHitDir(2);
				SOMA::PlaySound("spikeHit", 0 , 3);
				//std::cout << "set map object hit direction as right" << std::endl;
			}
			std::cout << "spike damage:" << mapObject->getDamage() << std::endl;
			obj->ChangeSoul(-mapObject->getDamage());
			obj->SetLastAttackedTime();
			std::cout << "Health: " << obj->GetSoul() << std::endl;
			break;
		}
	}
}

void CollisionManager::CheckEnemyMapDamage(const std::vector<MapObject*> mapObject, Enemies* obj)
{
	for (auto mapObject : mapObject) // For each platform.
	{
		SDL_FRect* temp = mapObject->GetDstP();
		if (COMA::AABBCheck(*obj->GetBody(), *temp) && mapObject->getIsHurt() == true)
		{
			obj->getDamage(mapObject->getDamage());
			break;
		}
	}
}