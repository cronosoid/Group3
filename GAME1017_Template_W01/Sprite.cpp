#include "Sprite.h"

void Sprite::Render()
{
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
	//SDL_RenderFillRectF(Engine::Instance().GetRenderer(), GetDstP());
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, SDL_FLIP_NONE);
}

void Sprite::SetSrcCords(int x, int y)
{
	m_src.x = x;
	m_src.y = y;
}

void Sprite::SetDstSize(int w, int h)
{
	m_dst.w = w;
	m_dst.h = h;
}

SDL_FPoint Sprite::GetCenter()
{
	return { m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2 };
}