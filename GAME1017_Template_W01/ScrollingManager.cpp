#include "ScrollingManager.h"

#include "MoveManager.h"
#include "TextureManager.h"

#include <iostream>

Background* SCMA::m_backgroundArr[2];
Background* SCMA::m_midgroundArr[2];
int SCMA::m_scrollWidth;

void ScrollingManager::Init()
{
	m_scrollWidth = static_cast<int>(WIDTH * 1.25);

	m_backgroundArr[0] = new Background({ 0,0,m_scrollWidth,768 }, { 0,0, (float)m_scrollWidth,768 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("lv_bg"));
	m_backgroundArr[1] = new Background({ 0,0,m_scrollWidth,768 }, { 1024,0,(float)m_scrollWidth,768 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("lv_bg"));

	m_midgroundArr[0] = new Background({ 0,0,m_scrollWidth,768 }, { 0,0, (float)m_scrollWidth * 0.8f,768 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("lv_mg"));
	m_midgroundArr[1] = new Background({ 0,0,m_scrollWidth,768 }, { (float)m_scrollWidth,0, (float)m_scrollWidth * 0.8f,768 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("lv_mg"));

}

void ScrollingManager::Update()
{
	float localTotalMove = -(MOMA::GetTotalMove().x - m_scrollWidth * ((int)MOMA::GetTotalMove().x / m_scrollWidth));
	m_backgroundArr[0]->GetDstP()->x = localTotalMove;
	m_backgroundArr[1]->GetDstP()->x = localTotalMove + m_scrollWidth;

	m_midgroundArr[0]->GetDstP()->x = localTotalMove * 0.8;
	m_midgroundArr[1]->GetDstP()->x = (localTotalMove + m_scrollWidth) * 0.8;
}

void ScrollingManager::Render()
{
	for (int i = 0; i < 2; i++)
	{
		m_backgroundArr[i]->Render();
	}

	for (int i = 0; i < 2; i++)
	{
		m_midgroundArr[i]->Render();
	}
}

void ScrollingManager::Clean()
{
	
}
