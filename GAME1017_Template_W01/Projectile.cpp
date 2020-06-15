#include "Projectile.h"
#include "Engine.h"
#include "States.h"

Projectile::Projectile(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, double spd, int face, double damage) :
	Sprite(s, d, r, t), m_speed(spd) {
	this->face = face;
	this->m_damage = damage;
}

void Projectile::Update()
{
	m_dst.x += m_speed * face;
}