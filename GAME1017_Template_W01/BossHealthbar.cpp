#include "BossHealthbar.h"

#include "TextureManager.h"

const float x = 250.0;
const float y = 10.0;
const float w = 500.0;
const float h = 35.0;

BossHealthbar::BossHealthbar(Enemies* boss) : Sprite({0,0,200,35},{x,y,w,h},Engine::Instance().GetRenderer(),TEMA::GetTexture("healthBar"))
{
	this->m_boss = boss;
	this->m_scale = new Sprite({ 0,0,200,35 }, { x,y,w,h }, Engine::Instance().GetRenderer(), TEMA::GetTexture("healthBarScale"));
}

BossHealthbar::~BossHealthbar()
{
	delete m_scale;
}

void BossHealthbar::Update()
{
	m_scale->GetDstP()->w = (m_boss->GetHealth() / (float)m_boss->GetMaxHealth()) * w;
}

void BossHealthbar::Render()
{
	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, SDL_FLIP_NONE);
	m_scale->Render();
}
