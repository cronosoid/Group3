#include <iostream>
#include "Button.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h"
#include "Level.h"
#include <SDL.h>

Button::Button(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t)
	: Sprite(src, dst, r, t), m_state(STATE_UP) {}

bool Button::MouseCollision()
{
	const int mx = EVMA::GetMousePos().x;
	const int my = EVMA::GetMousePos().y;
	return (mx < (m_dst.x + m_dst.w) && mx > m_dst.x &&
		    my < (m_dst.y + m_dst.h) && my > m_dst.y);
}

int Button::Update()
{
	bool col = MouseCollision();
	switch (m_state)
	{
	case STATE_UP:
		if (col)   
			m_state = STATE_OVER;
		break;
	case STATE_OVER:
		if (!col)
			m_state = STATE_UP;
		else if (col && EVMA::MousePressed(1))
			m_state = STATE_DOWN;
		break;
	case STATE_DOWN:
		if (EVMA::MouseReleased(1))
		{
			if (col)
			{
				m_state = STATE_OVER;
				// Execute new "callback".
				Execute();
				return 1;
			}
			else 
				m_state = STATE_UP;
		}
		break;
	}
	return 0;
}

void Button::Render()
{
	m_src.x = m_src.w * (int)m_state;
	SDL_RenderCopyF(m_pRend, m_pText, &m_src, &m_dst);
}

// Create button subclasses and their overridden Execute methods below...

PlayButtonFirst::PlayButtonFirst(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t) :Button(src, dst, r, t) {}
PlayButtonSecond::PlayButtonSecond(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t) : Button(src, dst, r, t) {}
RestartButton::RestartButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t) : Button(src, dst, r, t) {}
ExitButton::ExitButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t) : Button(src, dst, r, t) {}

void PlayButtonFirst::Execute()
{
	SOMA::Load("Aud/button.wav", "button", SOUND_SFX);
	SOMA::PlaySound("button");
	STMA::ChangeState(new GameState(new Level1()));
}

void PlayButtonSecond::Execute()
{
	SOMA::Load("Aud/button.wav", "button", SOUND_SFX);
	SOMA::PlaySound("button");
	STMA::ChangeState(new GameState(new Level2()));
}


void RestartButton::Execute()
{
	SOMA::PlaySound("button");
	STMA::ChangeState(new GameState(new Level1()));
}

void ExitButton::Execute()
{
	SOMA::PlaySound("button");
	Engine::Instance().setRunning(false);	
	exit(0);
}