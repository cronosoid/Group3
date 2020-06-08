#include "States.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h" // Make sure this is NOT in "States.h" or circular reference.
#include "TextureManager.h"
#include "Engine.h"
#include "Button.h"

#include <iostream>

// Begin State. CTRL+M+H and CTRL+M+U to turn on/off collapsed code.
void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}
void State::Resume() {}
// End State.

// Begin GameState.
GameState::GameState() {}

void GameState::Enter()
{
	std::cout << "Entering GameState..." << std::endl;
	m_pPlayer = new PlatformPlayer({ 0,0,0,0 }, { 512.0f,548.0f,50.0f,100.0f }, 
								   Engine::Instance().GetRenderer(), nullptr);
	m_pPlatforms[0] = new SDL_FRect({ 462.0f,648.0f,100.0f,20.0f });
	m_pPlatforms[1] = new SDL_FRect({ 200.0f,468.0f,100.0f,20.0f });
	m_pPlatforms[2] = new SDL_FRect({ 724.0f,468.0f,100.0f,20.0f });
	m_pPlatforms[3] = new SDL_FRect({ 462.0f,368.0f,100.0f,20.0f });
	m_pPlatforms[4] = new SDL_FRect({ -100.0f,668.0f,1224.0f,100.0f });
	SOMA::Load("Aud/jump.wav", "jump", SOUND_SFX);
}

void GameState::Update()
{
	// Get input.
	if (EVMA::KeyHeld(SDL_SCANCODE_A) && m_pPlayer->IsGrounded())
	{
		//walk left animation goes here
		m_pPlayer->SetAccelX(-1.0);
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_D) && m_pPlayer->IsGrounded())
	{
		//walk right animation goes here
		m_pPlayer->SetAccelX(1.0);
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_pPlayer->IsGrounded())
	{
		//jumping animation
		SOMA::PlaySound("jump");
		m_pPlayer->SetAccelY(-JUMPFORCE); // Sets the jump force.
		m_pPlayer->SetGrounded(false);
		
	}
	if (EVMA::KeyHeld(SDL_SCANCODE_J)) //melee
	{
		SDL_FRect rect;
		rect.x = m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w;
		rect.y = m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->w;
		rect.w = m_pPlayer->GetDstP()->w;
		rect.h = m_pPlayer->GetDstP()->h;

		//pass rect into collisioncheck while checking agaiinst an enemy

		if (COMA::AABBCheck(rect, *m_pPlatforms[2]))
		{

		}

		//m_pPlayer->Meele();
	}
	else if (EVMA::KeyHeld(SDL_SCANCODE_I)) // fireball
	{
		// will complete the projectile spawn in a while
		m_pPlayer->Fireball();
	}
	// Wrap the player on screen.
	if (m_pPlayer->GetDstP()->x < -51.0) m_pPlayer->SetX(1024.0);
	else if (m_pPlayer->GetDstP()->x > 1024.0) m_pPlayer->SetX(-50.0);
	// Do the rest.
	m_pPlayer->Update();
	CheckCollision();
}

void GameState::CheckCollision()
{
	for (int i = 0; i < NUMPLATFORMS; i++) // For each platform.
	{
		if (COMA::AABBCheck(*m_pPlayer->GetDstP(), *m_pPlatforms[i]))
		{
			if (m_pPlayer->GetDstP()->x + m_pPlayer->GetDstP()->w - m_pPlayer->GetVelX() <= m_pPlatforms[i]->x)
			{ // Collision from left.
				m_pPlayer->StopX(); // Stop the player from moving horizontally.
				m_pPlayer->SetX(m_pPlatforms[i]->x - m_pPlayer->GetDstP()->w);
			}
			else if (m_pPlayer->GetDstP()->x - (float)m_pPlayer->GetVelX() >= m_pPlatforms[i]->x + m_pPlatforms[i]->w)
			{ // Colliding right side of platform.
				m_pPlayer->StopX();
				m_pPlayer->SetX(m_pPlatforms[i]->x + m_pPlatforms[i]->w);
			}
			else if (m_pPlayer->GetDstP()->y + m_pPlayer->GetDstP()->h - (float)m_pPlayer->GetVelY() <= m_pPlatforms[i]->y)
			{ // Colliding top side of platform.
				m_pPlayer->SetGrounded(true);
				m_pPlayer->StopY();
				m_pPlayer->SetY(m_pPlatforms[i]->y - m_pPlayer->GetDstP()->h - 1);
			}
			else if (m_pPlayer->GetDstP()->y - (float)m_pPlayer->GetVelY() >= m_pPlatforms[i]->y + m_pPlatforms[i]->h)
			{ // Colliding bottom side of platform.
				m_pPlayer->StopY();
				m_pPlayer->SetY(m_pPlatforms[i]->y + m_pPlatforms[i]->h);
			}
		}
	}
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 128, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	// Draw the player.
	m_pPlayer->Render();
	// Draw the platforms.
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 192, 64, 0, 255);
	for (int i = 0; i < NUMPLATFORMS; i++)
		SDL_RenderFillRectF(Engine::Instance().GetRenderer(), m_pPlatforms[i]);
	// If GameState != current state.
	if (dynamic_cast<GameState*>(STMA::GetStates().back()))
		State::Render();
}

void GameState::Exit()
{
	delete m_pPlayer;
	for (int i = 0; i < NUMPLATFORMS; i++)
		delete m_pPlatforms[i];
}

void GameState::Resume() { }
// End GameState.

// Begin TitleState.
TitleState::TitleState() {}

void TitleState::Enter()
{
	m_playBtn = new PlayButton({ 0,0,400,100 }, { 312.0f,100.0f,400.0f,100.0f }, Engine::Instance().GetRenderer(), TEMA::GetTexture("play"));
	SOMA::Load("Aud/power.wav", "beep", SOUND_SFX);
}

void TitleState::Update()
{
	if (m_playBtn->Update() == 1)
		return; 
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 128, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	m_playBtn->Render();
	State::Render();
}

void TitleState::Exit()
{
	std::cout << "Exiting TitleState..." << std::endl;
}
// End TitleState.

// Begin EndState

EndState::EndState() {}

void EndState::Update()
{
	if (m_restartBtn->Update() == 1)
		return;
	if (m_exitBtn->Update() == 1)
		return;
}

void EndState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 128, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	m_restartBtn->Render();
	m_exitBtn->Render();
	State::Render();
}

void EndState::Enter()
{
}

void EndState::Exit()
{
	
}
