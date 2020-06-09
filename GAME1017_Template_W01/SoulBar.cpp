#include "SoulBar.h"
#include "MathManager.h"
#include "TextureManager.h"

void SoulBarBorder::Update()
{

}

void SoulBarBorder::Render()
{
	if (isDisplay == true)
	{
		SDL_RenderCopy(m_pRend, TextureManager::GetTexture("SoulBarBorder"), &m_src, &MAMA::ConvertFRect2Rect(m_dst));
	}
}

void SoulBarFiller::Update()
{
	//HeaPercent = hero->getHeaPercent();
	HeaPercent = 1.0;
}

void SoulBarFiller::Render()
{
	if (isDisplay == true)
	{
		m_dst.w = m_src.w * HeaPercent;
		SDL_RenderCopy(m_pRend, TextureManager::GetTexture("SoulBarFiller"), &m_src, &MAMA::ConvertFRect2Rect(m_dst));
	}
}





