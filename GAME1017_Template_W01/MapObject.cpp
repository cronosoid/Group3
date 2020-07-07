#include "MapObject.h"

MapObject::MapObject(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :Sprite(s, d, r, t)
{

}

SDL_FRect* MapObject::getFRect()
{
	return &m_dst;
}


Plate::Plate(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :MapObject(s, d, r, t)
{
	isHurt = false;
	canCollide = true;
	type = "Plate";
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
	isHurt = true;
	canCollide = false;
	type = "Spike";
}

void Spike::Update()
{

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


