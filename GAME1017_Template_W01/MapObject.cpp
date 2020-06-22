#include "MapObject.h"

MapObject::MapObject(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :Sprite(s, d, r, t)
{

}

bool MapObject::getIsHurt()
{
	return m_IsHurt;
}

bool MapObject::getCouldPass()
{
	return m_CouldPass;
}

int MapObject::getDamage()
{
	return m_Damage;
}


Plate::Plate(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :MapObject(s, d, r, t)
{
	m_IsHurt = false;
	m_CouldPass = false;
	m_Damage = 0;
}

void Plate::Update()
{

}

void Plate::Render()
{
	Sprite::Render();
}

Spike::Spike(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :MapObject(s, d, r, t)
{
	m_IsHurt = true;
	m_CouldPass = true;
	m_Damage = 10;
}

void Spike::Update()
{

}

void Spike::Render()
{
	Sprite::Render();
}



