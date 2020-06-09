#include "PlatformPlayer.h"
#include "Engine.h"
#include <algorithm>
#include <iostream>
#include "Utilities.h"

#include "TextureManager.h"

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t)
	:Entity(s, d, r, t)
{

	m_grounded = false;
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	m_maxVelX = 10.0;
	m_maxVelY = JUMPFORCE;
	m_grav = GRAV;
	m_drag = 0.88;

		m_curSoul = m_maxSoul;
}

void PlatformPlayer::Update()
{
	// Do X axis first.
	m_velX += m_accelX;
	m_velX *= (m_grounded ? m_drag : 1);
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	m_dst.x += (int)m_velX; // Had to cast it to int to get crisp collision with side of platform.
	// Now do Y axis.
	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav * 5));
	m_dst.y += (int)m_velY; // To remove aliasing, I made cast it to an int too.
	m_accelX = m_accelY = 0.0;
}

void PlatformPlayer::Render()
{
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
	//SDL_RenderFillRectF(Engine::Instance().GetRenderer(), GetDstP());
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, SDL_FLIP_NONE);
}

double PlatformPlayer::getHeaPercent()
{
	return m_curSoul / m_maxSoul;
}

double PlatformPlayer::GetSoul()
{
	return m_curSoul;
}

void PlatformPlayer::SoulRcvry()
{
	m_curSoul += m_soulRecover;
	if (m_curSoul > m_maxSoul)
		m_curSoul = m_maxSoul;
}


void PlatformPlayer::Meele()
{

	std::cout << "meele\n";

}
