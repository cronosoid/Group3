#pragma once
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Sprite.h"
#include <SDL.h>

// Button will be an example of the command design pattern.
class Button : public Sprite
{
public:
	int Update();
	void Render();
protected: // Private but inherited!
	Button(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
	enum state { STATE_UP, STATE_OVER, STATE_DOWN } m_state;
	bool MouseCollision();
	virtual void Execute() = 0; // Pure virtual method, meaning Button is now an abstract class (interface)!
};

// List button subclasses here...

class PlayButtonFirst : public Button
{
private:
	void Execute();
public:
	PlayButtonFirst(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
};

class PlayButtonSecond : public Button
{
private:
	void Execute();
public:
	PlayButtonSecond(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
};

class RestartButton : public Button
{
private:
	void Execute();
public:
	RestartButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
};

class ExitButton : public Button
{
private:
	void Execute();
public:
	ExitButton(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t);
};


#endif