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

	for (Enemies* enemy : EnemiesVec)
	{
		if (COMA::AABBCheck(*this->GetDstP(), *enemy->GetDstP()))
		{
			enemy->getDamage(hero->m_magicDmg);
			enemy->Stun(MELEESTUNTIME);
			this->isActive = false;
			break;
		}
	}

	for (MapObject* object : mapObjects)
	{
		if (COMA::AABBCheck(*this->GetDstP(), *object->GetDstP()))
		{
			this->isActive = false;
			break;
		}
	}
}
