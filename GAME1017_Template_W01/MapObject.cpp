#include "MapObject.h"

#include <iostream>

MapObject::MapObject(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :Sprite(s, d, r, t)
{
	m_CanMove = false;
	m_isActive = true;
}

MapObject::MapObject(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, SDL_FPoint start, SDL_FPoint end) : Sprite(s, d, r, t)
{
	m_startPoint = start;
	m_endPoint = end;
	m_CanMove = true;
	m_mDir = true;
	m_isActive = true;
}


bool MapObject::getActive()
{
	return m_isActive;
}

bool MapObject::getIsHurt()
{
	return m_IsHurt;
}

bool MapObject::getCanCollide()
{
	return m_CanCollide;
}

bool MapObject::getCanMove()
{
	return m_CanMove;
}

int MapObject::getDamage()
{
	return m_Damage;
}

void MapObject::changeDir()
{
	m_mDir = !m_mDir;
	/*if (m_mDir)
		std::cout << "Change Direction to end" << std::endl;
	else
		std::cout << "Change Direction to start" << std::endl;*/
}

void MapObject::moveStart(double x,double y)
{
	m_startPoint.x += x;
	m_startPoint.y += y;
}


void MapObject::moveEnd(double x,double y)
{
	m_endPoint.x += x;
	m_endPoint.y += y;
}

void MapObject::moveMapObject()
{
	if (m_CanMove == true)
	{
		switch (m_mDir)
		{
		case true://to end
		{
			if (m_startPoint.x > m_endPoint.x && m_startPoint.y == m_endPoint.y)//move left
			{
				m_dst.x -= m_moveSpeed;
				if (m_dst.x <= m_endPoint.x)
				{
					m_dst.x = m_endPoint.x;
					changeDir();
				}
			}
			else if (m_startPoint.x < m_endPoint.x && m_startPoint.y == m_endPoint.y)//move right
			{
				m_dst.x += m_moveSpeed;
				if (m_dst.x >= m_endPoint.x)
				{
					m_dst.x = m_endPoint.x;
					changeDir();
				}
			}
			else if (m_startPoint.x == m_endPoint.x && m_startPoint.y > m_endPoint.y)//move up
			{
				m_dst.y -= m_moveSpeed;
				if (m_dst.y <= m_endPoint.y)
				{
					m_dst.x = m_endPoint.x;
					changeDir();
				}
			}
			else if (m_startPoint.x == m_endPoint.x && m_startPoint.y < m_endPoint.y)//move down
			{
				m_dst.y += m_moveSpeed;
				if (m_dst.y >= m_endPoint.y)
				{
					m_dst.x = m_endPoint.x;
					changeDir();
				}
			}
			break;
		}
		case false://to start
		{
			if (m_startPoint.x < m_endPoint.x && m_startPoint.y == m_endPoint.y)//move left
			{
				m_dst.x -= m_moveSpeed;
				if (m_dst.x <= m_startPoint.x)
				{
					m_dst.x = m_startPoint.x;
					changeDir();
				}
			}
			else if (m_startPoint.x > m_endPoint.x && m_startPoint.y == m_endPoint.y)//move right
			{
				m_dst.x += m_moveSpeed;
				if (m_dst.x >= m_startPoint.x)
				{
					m_dst.x = m_startPoint.x;
					changeDir();
				}
			}
			else if (m_startPoint.x == m_endPoint.x && m_startPoint.y < m_endPoint.y)//move up
			{
				m_dst.y -= m_moveSpeed;
				if (m_dst.y <= m_startPoint.y)
				{
					m_dst.x = m_startPoint.x;
					changeDir();
				}
			}
			else if (m_startPoint.x == m_endPoint.x && m_startPoint.y > m_endPoint.y)//move down
			{
				m_dst.y += m_moveSpeed;
				if (m_dst.y >= m_startPoint.y)
				{
					m_dst.x = m_startPoint.x;
					changeDir();
				}
			}
			break;
		}
		default:break;
		}
		//std::cout << m_dst.x << " " << m_dst.y << std::endl;
	}	
}

Plate::Plate(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :MapObject(s, d, r, t)
{
	m_IsHurt = false;
	m_CanCollide = true;
	m_Damage = 0;
  type = "Plate";
}

Plate::Plate(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, SDL_FPoint start, SDL_FPoint end):MapObject(s, d, r, t,start,end)
{
	m_IsHurt = false;
	m_CanCollide = true;
	m_Damage = 0;
	type = "Plate";
	m_moveSpeed = MOVESPEED;
}


void Plate::Update()
{
	moveMapObject();
}

void Plate::Render()
{
	Sprite::Render();
}

Spike::Spike(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :MapObject(s, d, r, t)
{
	m_IsHurt = true;
	m_CanCollide = false;
	m_Damage = 10;
	type = "Spike";
}

Spike::Spike(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, SDL_FPoint start, SDL_FPoint end) :MapObject(s, d, r, t, start, end)
{
	m_IsHurt = true;
	m_CanCollide = false;
	m_Damage = 10;
	type = "Spike";
	m_moveSpeed = MOVESPEED;
}


void Spike::Update()
{
	moveMapObject();
}

void Spike::Render()
{
	Sprite::Render();
}

Portal::Portal(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t):MapObject(s, d, r, t)
{
	m_IsHurt = false;
	m_CanCollide = false;
	m_Damage = 10;
	type = "Portal";
}

void Portal::Update()
{
	
}

void Portal::Render()
{
	Sprite::Render();
}

UnstableBrick::UnstableBrick(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :MapObject(s, d, r, t)
{
	m_IsHurt = false;
	m_CanCollide = true;
	m_Damage = 0;
	type = "UnstableBrick";
	m_status = 1;//normal

	m_brokenTime = BREAKTIME;
}

UnstableBrick::UnstableBrick(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, SDL_FPoint start, SDL_FPoint end):MapObject(s,d,r,t,start,end)
{
	m_IsHurt = false;
	m_CanCollide = true;
	m_Damage = 0;
	type = "UnstableBrick";	
	m_status = 1;//normal

	m_brokenTime = BREAKTIME;
	m_moveSpeed = MOVESPEED;
}


void UnstableBrick::Update()
{
	moveMapObject();
	if (m_status == 2)//breaking
	{
		//m_brokenTime--;
		////std::cout << "breaking" << m_brokenTime<< std::endl;
		//if(m_brokenTime==0)
		//{
		//	m_isActive = false;
		//}
		m_dst.y += 2.0f;
		if (m_dst.y >= 768.0f + 128.0f)
			m_isActive = false;
	}
}

void UnstableBrick::Render()
{
	Sprite::Render();
}

void UnstableBrick::changeStatus(int state)
{
	m_status = state;
}

int UnstableBrick::getStatus()
{
	return m_status;
}


