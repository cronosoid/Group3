#include "CDBar.h"
#include "MathManager.h"
#include "TextureManager.h"

void CDBarBorder::Update()
{
	/*if ((this->changeTime + animFramerate * 1000) < SDL_GetTicks())
	{
		this->changeTime = SDL_GetTicks();
		this->animFrame++;
		if (this->animFrame > this->maxAnimFrame)
			this->animFrame = 0;
		this->SetSrcCords(this->GetSrcP()->w * this->animFrame, 0);
	}*/
}

void CDBarBorder::Render()
{
	if (isDisplay == true)
	{
		SDL_RenderCopy(m_pRend, TextureManager::GetTexture("SoulBarBorder"), &m_src, &MAMA::ConvertFRect2Rect(m_dst));
	}
}

void CDBarFiller::Update()
{
	this->SetDstSize(this->maxWidth * (1000 / hero->getMagicTime()), this->m_dst.h);
	//std::cout << this->maxWidth * (hero->GetSoul() / hero->m_maxSoul) << std::endl;
}

void CDBarFiller::Render()
{
	if (isDisplay == true)
	{
		SDL_RenderCopy(m_pRend, TextureManager::GetTexture("SoulBarFiller"), &m_src, &MAMA::ConvertFRect2Rect(m_dst));
	}
}