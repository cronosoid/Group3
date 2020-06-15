#include "Entity.h"

Entity::Entity(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t)
	:Sprite(s, d, r, t)
{
	m_grounded = false;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 10.0;
	m_maxVelY = JUMPFORCE;
	m_grav = GRAV;
	m_drag = 0.88;
}

void Entity::Stop() // If you want a dead stop both axes.
{
	StopX();
	StopY();
}
void Entity::StopX() { m_velX = 0.0; }
void Entity::StopY() { m_velY = 0.0; }

void Entity::SetAccelX(double a) { m_accelX = a; }
void Entity::SetAccelY(double a) { m_accelY = a; }
bool Entity::IsGrounded() { return m_grounded; }
void Entity::SetGrounded(bool g, SDL_FRect* Floor) {
	this->m_grounded = g;
	this->Floor = Floor;
}
double Entity::GetVelX() { return m_velX; }
double Entity::GetVelY() { return m_velY; }
void Entity::SetX(float y) { m_dst.x = y; }
void Entity::SetY(float y) { m_dst.y = y; }