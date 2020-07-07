#pragma once
#include <vector>
#include "Sprite.h"
#include "PlatformPlayer.h"
#include "Enemies.h"
#include "MapObject.h"

class Projectile : public Sprite
{
protected:
	double m_speed;
	int face;
	double m_damage;
	int m_curFrame = 0;
	int m_curAnimFrame = 0;
	int m_framesFrequency;
	int m_maxFrames;
	int m_moveX;
	int m_moveY;
	/*PlatformPlayer* hero;
	std::vector<Enemies*> *EnemiesVec;
	std::vector<MapObject*> *MapObjVec;*/
	bool isActive;
public:
	Projectile(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double spd, int face, double damage = 0,
		int framesFrequency = 0, int maxFrames = 0, int m_moveX = 0, int m_moveY = 0);
	virtual void Update();
	bool getIsActive() { return isActive; }
};
