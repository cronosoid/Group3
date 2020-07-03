#include "CollisionManager.h"
#include "DebugManager.h"

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

void CollisionManager::CheckMapCollision(const std::vector<MapObject*> mapObjects, Entity* obj)
{
	obj->SetGrounded(false);
	for (MapObject* mapObject : mapObjects) // For each platform.
	{
		SDL_FRect* mapObjectRect = mapObject->getFRect();
		if (mapObject->getCollision() and COMA::AABBCheck(*obj->GetDstP(), *mapObjectRect))
		{
			if (obj->GetDstP()->y + obj->GetDstP()->h - (float)obj->GetVelY() <= mapObjectRect->y)
			{ // Colliding top side of platform.
				obj->SetGrounded(true, mapObjectRect);
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