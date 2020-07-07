#include "MapObject.h"

#include <iostream>

MapObject::MapObject(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :Sprite(s, d, r, t)
{
	m_CanMove = false;	
}

MapObject::MapObject(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, SDL_FPoint start, SDL_FPoint end) : Sprite(s, d, r, t)
{
	m_startPoint = start;
	m_endPoint = end;
	m_CanMove = true;
	m_mDir = true;
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
	m_moveSpeed = MOVESPEED;
}


void Plate::Update()
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
	}
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
	m_moveSpeed = MOVESPEED;
}


void Spike::Update()
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
	}
}

void Spike::Render()
{
	Sprite::Render();
}

Portal::Portal(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t):MapObject(s, d, r, t)
{
	isHurt = false;
	canCollide = false;
	type = "Portal";
}

void Portal::Update()
{
	
}

void Portal::Render()
{
	Sprite::Render();
}


