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
	couldPass = false;
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
	couldPass = true;
}

void Spike::Update()
{

}

void Spike::Render()
{
	Sprite::Render();
}



