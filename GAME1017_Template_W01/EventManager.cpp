#include "EventManager.h"
#include "Engine.h"
#include <cstring>
#include <iostream>

void EventManager::Init()
{
	s_keysCurr = SDL_GetKeyboardState(&s_numKeys);
	s_keysLast = new Uint8[s_numKeys];
	std::memcpy(s_keysLast, s_keysCurr, s_numKeys);
	s_mouseCurr = SDL_GetMouseState(&s_mousePos.x, &s_mousePos.y);
	s_mouseLast = s_mouseCurr;
	std::cout << "EventManager init done!" << std::endl;
}

void EventManager::HandleEvents()
{
	SDL_Event event;
	
	std::memcpy(s_keysLast, s_keysCurr, s_numKeys);
	s_mouseLast = s_mouseCurr;
	s_lastKeyDown = s_lastKeyUp = -1;

	while (SDL_PollEvent(&event)) // Pump events invoked.
	{
		switch (event.type) // Parse some global events.
		{
			case SDL_QUIT: // User pressed window's 'x' button.
				Engine::Instance().Running() = false;
				break;
			case SDL_KEYDOWN:
				s_lastKeyDown = event.key.keysym.sym;
				break;
			case SDL_KEYUP:
				s_lastKeyUp = event.key.keysym.sym;
				if (event.key.keysym.sym == SDLK_ESCAPE)
					Engine::Instance().Running() = false;
				break;
		}
	}
	s_keysCurr = SDL_GetKeyboardState(&s_numKeys);
	s_mouseCurr = SDL_GetMouseState(&s_mousePos.x, &s_mousePos.y);
}

bool EventManager::KeyHeld(const SDL_Scancode c)
{
	if (s_keysCurr != nullptr)
	{
		if (s_keysCurr[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

bool EventManager::KeyPressed(const SDL_Scancode c)
{
	return (s_keysCurr[c] > s_keysLast[c]);
}

bool EventManager::KeyReleased(const SDL_Scancode c)
{
	return (s_keysCurr[c] < s_keysLast[c]);
}

int EventManager::LastKeyDown()
{
	return s_lastKeyDown;
}

int EventManager::LastKeyUp()
{
	return s_lastKeyUp;
}

bool EventManager::MouseHeld(const int b)
{
	if (b >= 1 && b <= 3)
		return (s_mouseCurr & SDL_BUTTON(b));
	else
		return false;
}

bool EventManager::MousePressed(const int b)
{
	return ((s_mouseCurr & SDL_BUTTON(b)) > (s_mouseLast & SDL_BUTTON(b)));
}

bool EventManager::MouseReleased(const int b)
{
	return ((s_mouseCurr & SDL_BUTTON(b)) < (s_mouseLast & SDL_BUTTON(b)));
}

SDL_Point& EventManager::GetMousePos()
{
	return s_mousePos;
}

void EventManager::Quit()
{
	delete s_keysLast;
}

const Uint8* EventManager::s_keysCurr = nullptr;
Uint8* EventManager::s_keysLast;
int EventManager::s_numKeys;

int EventManager::s_lastKeyDown;
int EventManager::s_lastKeyUp;
SDL_Point EventManager::s_mousePos = { 0,0 };
Uint32 EventManager::s_mouseCurr;
Uint32 EventManager::s_mouseLast;