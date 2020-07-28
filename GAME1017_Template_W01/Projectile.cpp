#include "Projectile.h"
#include "Engine.h"
#include "States.h"
#include "CollisionManager.h"
#include "Enemies.h"
#include "MapObject.h"

Projectile::Projectile(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double spd, int face, double damage, int framesFrequency, int maxFrames, int moveX, int moveY):
	Sprite(s, d, r, t), m_speed(spd)
{
	this->face = face;
	this->m_damage = damage;
	this->m_framesFrequency = framesFrequency;
	this->m_maxFrames = maxFrames;
	this->m_moveX = moveX;
	this->m_moveY = moveY;
	isActive = true;
}


void Projectile::Update()
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
}