#include "Enemies.h"
#include <algorithm>
#include <iostream>
#include "Engine.h"

const double GRAV = 6.0;
const double JUMPFORCE = 60.0;

Enemies::Enemies(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t)
	:Sprite(s, d, r, t)
{
	m_grounded = false;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 10.0;
	m_maxVelY = JUMPFORCE;
	m_grav = GRAV;
	m_drag = 0.88;
	isActive = true;
	isAlive = true;
}

void Enemies::Stop() // If you want a dead stop both axes.
{
	StopX();
	StopY();
}
void Enemies::StopX() { m_velX = 0.0; }
void Enemies::StopY() { m_velY = 0.0; }

void Enemies::SetAccelX(double a) { m_accelX = a; }
void Enemies::SetAccelY(double a) { m_accelY = a; }
bool Enemies::IsGrounded() { return m_grounded; }
void Enemies::SetGrounded(bool g) { m_grounded = g; }
double Enemies::GetVelX() { return m_velX; }
double Enemies::GetVelY() { return m_velY; }
void Enemies::SetX(float y) { m_dst.x = y; }
void Enemies::SetY(float y) { m_dst.y = y; }
void Enemies::setActive(bool a) { isActive = a; }
bool Enemies::getActive() { return isActive; }
bool Enemies::getAlive(){return isAlive;}
void Enemies::setAlive(bool a) { isActive = a; }
SDL_Texture* Enemies::getTex() { return m_pText; }
std::string Enemies::getType() { return EnemyType; }


void Enemies::decHealth(int attack)
{
	Health -= attack;
}


