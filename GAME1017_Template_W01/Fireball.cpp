#include "Fireball.h"
#include "Engine.h"
#include "States.h"

Fireball::Fireball(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int spd, int face) :
	Sprite(s, d, r, t), m_speed(spd) {
	this->face = face;
}

void Fireball::Update()
{
	m_dst.x += m_speed * face;
}