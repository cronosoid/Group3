#include "Arrow.h"

#include <ctime>

#include "CollisionManager.h"
#include "Archer.h"
#include "SoundManager.h"

const int ARCHERDAMAGE = 10;

Arrow::Arrow(PlatformPlayer* hero, std::vector<MapObject*> mabObjects, SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double spd, int face, double damage, int framesFrequency, int maxFrames, int m_moveX, int m_moveY) :
	Projectile(s, d, r, t, spd, face, damage, framesFrequency, maxFrames, m_moveX, m_moveY)
{
	this->hero = hero;
	this->mapObjects = mapObjects;
	this->SetAngle(180 * (face - 1) / 2 );
}

void Arrow::Update()
{	
	m_dst.x += m_speed * face;
	
	if (m_curFrame >= m_framesFrequency)
	{
		m_curFrame = 0;
		m_curAnimFrame++;
		if (m_curAnimFrame >= m_maxFrames)
		{
			m_curAnimFrame = 0;
		}

		m_src.x = m_moveX * m_curAnimFrame;
		if (face == -1)
			m_src.y = m_moveY;
	}
	else
	{
		m_curFrame++;
	}
	
	if (COMA::AABBCheck(*this->GetDstP(), *hero->GetBody())
		and hero->GetLastAttackedTime() <= 0)
	{
		if (face == 1)
		{
			hero->setHitDir(1);//set hit direction as left
		}
		else
		{
			hero->setHitDir(2);//set hit direction as right
		}
		hero->getDamage(ARCHERDAMAGE);
		hero->SetLastAttackedTime();
		hero->StopX();
		SOMA::PlaySound("arrowHit");
		isActive = false;
		return;
	}

	for (int i = 0; i < (int)mapObjects.size(); i++)
	{
		if (COMA::AABBCheck(*this->GetDstP(), *mapObjects[i]->GetDstP()))
		{
			this->isActive = false;
			SOMA::PlaySound("arrowHit");
			break;
		}
	}
}
