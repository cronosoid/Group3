#include "Fireball.h"
#include "CollisionManager.h"

Fireball::Fireball(PlatformPlayer* hero, std::vector<Enemies*> EnemiesVec, std::vector<MapObject*> mabObjects, SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double spd, int face, double damage, int framesFrequency, int maxFrames, int m_moveX, int m_moveY):
Projectile(s,d,r,t, spd, face, damage, framesFrequency, maxFrames, m_moveX, m_moveY)
{
	this->hero = hero;
	this->EnemiesVec = EnemiesVec;
}

void Fireball::Update()
{
	Projectile::Update();

	for (int i = 0; i < (int)EnemiesVec.size(); i++)
	{
		if (COMA::AABBCheck(*this->GetDstP(), *EnemiesVec[i]->GetDstP()))
		{
			EnemiesVec[i]->getDamage(hero->m_magicDmg);
			this->isActive = false;
			break;
		}
	}

	for (int i = 0; i <(int)mapObjects.size(); i++)
	{
		if (COMA::AABBCheck(*this->GetDstP(), *mapObjects[i]->GetDstP()))
		{
			this->isActive = false;
			break;
		}
	}
}
