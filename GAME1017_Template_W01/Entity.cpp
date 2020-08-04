#include "Entity.h"
#include <algorithm>
#include "Engine.h"
#include "MoveManager.h"
#include "TextureManager.h"

Entity::Entity(SDL_Rect s, SDL_FRect d, SDL_FRect b, SDL_Renderer* r, SDL_Texture* t, Animator* animator, bool player)
	:Sprite(s, d, r, t)
{
	this->m_grounded = false;
	this->m_accelX = this->m_accelY = this->m_velX = this->m_velY = 0.0;
	this->m_maxVelX = 10.0;
	this->m_maxJumpVelocity = MAXFLYPOWER;
	this->m_maxFallVelocity = FALLCOF;
	this->m_grav = GRAV;
	this->m_drag = 0.88;
	this->flyingTime = 0;
	this->animator = animator;
	this->m_player = player;
	this->globalDst = {d.x + MOMA::GetTotalMove().x, d.y + MOMA::GetTotalMove().y, d.w, d.h};

	this->m_body = {d.x,d.y,b.w,b.h};
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
void Entity::SetGrounded(bool g, MapObject* Floor) {
	this->m_grounded = g;
	this->m_floor = Floor;
}
double Entity::GetVelX() { return m_velX; }
double Entity::GetVelY() { return m_velY; }
void Entity::SetX(float y) { m_body.x = y; }
void Entity::SetY(float y) { m_body.y = y; }

void Entity::movementUpdate()
{
	SDL_FRect& body = m_body;
	
	double gravAcceleration = 1;

	if (flyingTime > FPS*0.5)
	{
		gravAcceleration = 1.0 + flyingTime / (double)FPS;
		gravAcceleration = std::min(std::max(gravAcceleration,1.0),2.5);
	}
	m_grounded == false ? flyingTime++ : flyingTime = 0;

	// Do X axis first.
	this->m_velX += this->m_accelX;
	this->m_velX *= (this->m_grounded ? this->m_drag : 1);
	this->m_velX = std::min(std::max(this->m_velX, -this->m_maxVelX), (this->m_maxVelX));

	// Now do Y axis.

	this->m_velY += this->m_accelY + this->m_grav * gravAcceleration; // Adjust gravity to get slower jump.
	this->m_velY = std::min(std::max(this->m_velY, -this->m_maxJumpVelocity), (this->m_grav * this->m_maxFallVelocity));
	
	if (this->m_player and MoveManager::CheckXBoundaries(body.x, (int)this->m_velX, this))
	{
		MoveManager::MoveX((int)this->m_velX);
	}
	else
	{
		body.x += (int)this->m_velX;
	}
	this->getGlobalDst()->x = m_body.x + MOMA::GetTotalMove().x;

	if (this->m_player and MoveManager::checkYBoundaries(body.y, (int)this->m_velY, this))
	{
		MoveManager::MoveY((int)this->m_velY);
	}
	else
	{
		body.y += (int)this->m_velY;
	}

	if (this->m_player)
	{
		body.x = std::min(std::max(body.x, 0.0f), (float)MOMA::GetRightBorder() - m_body.w);
	}

	if (abs(this->m_velX) < 0.001f)
		this->m_velX = 0;
	if (abs(this->m_velY) < 0.001f)
		this->m_velY = 0;
	
	this->m_accelX = this->m_accelY = 0.0;

	this->m_dst.x = this->m_body.x + (this->m_body.w - this->m_dst.w) / 2;
	this->m_dst.y = this->m_body.y + (this->m_body.h - this->m_dst.h);
}

void Entity::addAnimator(Animator* animator)
{
	if (this->animator == nullptr)
		this->animator = animator;
}
