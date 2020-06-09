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
	this->SetDstSize(this->maxWidth*(hero->GetSoul()/hero->m_maxSoul),this->m_dst.h);
	//std::cout << this->maxWidth * (hero->GetSoul() / hero->m_maxSoul) << std::endl;
}

void SoulBarFiller::Render()
{
	if (isDisplay == true)
	{
		SDL_RenderCopy(m_pRend, TextureManager::GetTexture("SoulBarFiller"), &m_src, &MAMA::ConvertFRect2Rect(m_dst));
	}
}