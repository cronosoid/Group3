#include "Engine.h"
#include "CollisionManager.h"
#include "DebugManager.h"
#include "EventManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "MoveManager.h"

#include <iostream>
#define WIDTH 1024
#define HEIGHT 768

using namespace std;

Engine::Engine() :m_running(false), m_pBNull(false) { cout << "Engine class constructed!" << endl; }

bool Engine::Init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	cout << "Initializing game..." << endl;
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window.
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				EVMA::Init();
				SOMA::Init();
				TEMA::Init();
			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	TEMA::RegisterTexture("Img/background.png", "background");
	TEMA::RegisterTexture("Img/end_background.png", "end_background");
	TEMA::RegisterTexture("Img/click.png", "click");
	TEMA::RegisterTexture("Img/play.png", "play");
	TEMA::RegisterTexture("Img/back.png", "back");
	TEMA::RegisterTexture("Img/help.png", "help");
	TEMA::RegisterTexture("Img/exit.png", "exit");
	TEMA::RegisterTexture("Img/resume.png", "resume");
	TEMA::RegisterTexture("Img/mainmenu.png", "mainmenu");
	TEMA::RegisterTexture("Img/restart.png", "restart");
	TEMA::RegisterTexture("Img/NxtLvl.png", "nextLevel");
	TEMA::RegisterTexture("Img/congratulations.png", "congratulations");
	TEMA::RegisterTexture("Img/Fireball_sheet.png", "fireball");	
	TEMA::RegisterTexture("Img/HighlightedBrick.png", "hBrick");
	TEMA::RegisterTexture("Img/HealthBar.png", "healthBar");
	TEMA::RegisterTexture("Img/HealthBarScale.png", "healthBarScale");
	TEMA::RegisterTexture("../Spritesheets/Arrow.png", "Arrow");
	TEMA::RegisterTexture("../Spritesheets/Kaben_Sheet.png", "KabenSheet"); 
	TEMA::RegisterTexture("../Spritesheets/Swordman_sheet.png", "Swordman");
	TEMA::RegisterTexture("../Spritesheets/Archer_sheet.png", "Archer");
	TEMA::RegisterTexture("../Spritesheets/Boss_sheet.png", "Boss");
	
	FOMA::RegisterFont("Img/LTYPE.TTF", "font", 20);

	FOMA::RegisterFont("Img/LTYPE.ttf", "Ltype", 24);
	SOMA::Load("Aud/button.wav", "button", SOUND_SFX);
	
	STMA::ChangeState(new ClickState);
	SOMA::AllocateChannels(16);
	m_running = true; // Everything is okay, start the engine.
	cout << "Engine Init success!" << endl;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) // Engine has to sleep.
		SDL_Delay(m_fps - m_delta);
}

void Engine::HandleEvents()
{
	EVMA::HandleEvents();
}

void Engine::Update()
{
	STMA::Update();
}

void Engine::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	// Draw anew.
	STMA::Render();
	SDL_RenderPresent(m_pRenderer);
}

void Engine::Clean()
{
	cout << "Cleaning game." << endl;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	DEMA::Quit();
	EVMA::Quit();
	FOMA::Quit();
	SOMA::Quit();
	STMA::Quit();
	TEMA::Quit();
	IMG_Quit();
	SDL_Quit();
}

int Engine::Run()
{
	if (m_running) // What does this do and what can it prevent?
		return -1;
	if (Init("Mastiv First", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;
	while (m_running) // Main engine loop.
	{
		Wake();
		HandleEvents();
		Update();
		Render();
		if (m_running)
			Sleep();
	}
	Clean();
	return 0;
}

Engine& Engine::Instance()
{
	static Engine instance; // C++11 will prevent this line from running more than once. Magic statics.
	return instance;
}

SDL_Renderer* Engine::GetRenderer() { return m_pRenderer; }
bool& Engine::Running() { return m_running; }


bool& Engine::isNull() { return m_pBNull; }
void Engine::setNull() { m_pBNull = true; }

void Engine::setRunning(bool x)
{
	m_running = x;
}