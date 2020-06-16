#include "PlatformPlayer.h"
#include "Engine.h"
#include <algorithm>
#include <iostream>

#include "TextureManager.h"

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, Animator* animator)
	:Entity(s, d, r, t, animator)
{
	m_curSoul = m_maxSoul;
}

void PlatformPlayer::Update()
{
	movementUpdate();
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

void PlatformPlayer::ChangeSoul(int amnt)
{
	m_curSoul += amnt;
	if (m_curSoul > m_maxSoul)
		m_curSoul = m_maxSoul;
	else if (m_curSoul < 0)
		m_curSoul = 0;
}

void PlatformPlayer::Meele()
{

	std::cout << "meele\n";

}
