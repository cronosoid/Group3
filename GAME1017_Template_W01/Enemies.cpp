#include "Enemies.h"
#include <algorithm>
#include <iostream>

#include "CollisionManager.h"
#include "EnemyManager.h"
#include "Engine.h"

Enemies::Enemies(SDL_Rect s, SDL_FRect d, SDL_FRect b, SDL_Renderer* r, SDL_Texture* t, Animator* animator)
	:Entity(s, d, b, r, t, animator)
{
	isActive = true;
	isAlive = true;
	m_playerLOS = false;
	m_stunTime = 0;
	m_lastDetectTime = 0;
	m_hided = 0;
	m_waitBehindTheCover = 0;
	m_noLOStime = 0;
}

void Enemies::setActive(bool a) { isActive = a; }

void Enemies::Flee(const float RUNSPEED, const float squareDistToPlayer, const float ATTACKDISTANCE, const float STOPDISTANCE)
{
	PlatformPlayer* player = EnemyManager::GetTarget();
	
	this->m_speed = RUNSPEED + (rand() % 10) / 10.0;

	float dist = (player->GetBody()->x + player->GetBody()->w / 2) - (this->m_body.x - this->m_body.w / 2);

	float direction = 1;
	if (dist != 0)
		direction = abs(dist) / dist;

	if (direction == 1)
		animator->setFace(1);
	else if (direction == -1)
		animator->setFace(0);

	float curX;
	animator->getFace() == 0 ? curX = m_body.x + m_body.w + 5 : curX = m_body.x - 5;
	float curY = m_body.y;
	MapObject* nextObject = COMA::FindFirstObjectOnTheRay({ curX,curY }, { 0, 1 });

	if (nextObject and not nextObject->getIsHurt())
	{
		SetAccelX(-direction * m_speed);
		if (m_floor and nextObject->GetDstP()->y < m_floor->GetDstP()->y)
		{
			this->SetAccelY(-JUMPFORCE);
		}
	}
}

void Enemies::Seek(const float RUNSPEED, const float squareDistToPlayer, const float STOPDISTANCE, const float ATTACKDISTANCE, bool knowWherePlayer)
{
	PlatformPlayer* player = EnemyManager::GetTarget();
	
	this->m_speed = RUNSPEED + (rand() % 10) / 10.0;

	float dist = (player->GetBody()->x + player->GetBody()->w / 2) - (this->m_body.x + this->m_body.w / 2);

	float direction = 1;
	if (dist != 0)
	{
		direction = abs(dist) / dist;
	}
	if (direction == 1)
		animator->setFace(0);
	else if (direction == -1)
		animator->setFace(1);

	float curX;
	animator->getFace() == 0 ? curX = m_body.x + m_body.w + 5 : curX = m_body.x - 5;
	float curY = m_body.y;
	MapObject* nextObject = COMA::FindFirstObjectOnTheRay({ curX,curY }, { 0, 1 });

	if (nextObject and not nextObject->getIsHurt() and squareDistToPlayer > pow(STOPDISTANCE, 2) and knowWherePlayer)
	{
		SetAccelX(direction * m_speed);
		if (m_floor and nextObject->GetDstP()->y < m_floor->GetDstP()->y)
		{
			this->SetAccelY(-JUMPFORCE);
		}
	}
}

void Enemies::Hide(const float RUNSPEED, const float squareDistToPlayer, const float ATTACKDISTANCE, const float STOPDISTANCE)
{
	PlatformPlayer* player = EnemyManager::GetTarget();
	
	if (m_waitBehindTheCover == 0)
	{
		if (m_playerLOS)
		{
			Flee(RUNSPEED, squareDistToPlayer, ATTACKDISTANCE, STOPDISTANCE);
			m_noLOStime = 0;
		}
		else
			m_noLOStime++;

		if (m_noLOStime > FPS * 0.5)
			m_waitBehindTheCover = FPS * HIDETIME;
	}
	else
	{
		if (m_waitBehindTheCover-- == 1 or m_playerLOS)
		{
			m_hided = FPS * HIDECOOLDOWN;
			m_noLOStime = 0;
		}
	}
}

bool Enemies::getActive() { return isActive; }
bool Enemies::getAlive() { return isAlive; }
void Enemies::setAlive(bool a) { isActive = a; }
SDL_Texture* Enemies::getTex() { return m_pText; }
std::string Enemies::getType() { return enemyType; }

void Enemies::getDamage(int damage)
{
	health -= damage;
}