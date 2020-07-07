#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#define NUMENEMIES 2
#define NUMPLATFORMS 5

#include <SDL.h>
#include "Button.h"
#include "Background.h"
#include "PlatformPlayer.h"
#include "Animator.h"
#include "Level.h"
#include <vector>

class State // This is the abstract base class for all specific states.
{
protected:
	State() {}
public:
	virtual void Update() = 0; // Having at least one 'pure virtual' method like this, makes a class abtract.
	virtual void Render();     // Meaning we cannot create objects of the class.
	virtual void Enter() = 0;  // Virtual keyword means we can override in derived class.
	virtual void Exit() = 0;
	virtual void Resume();
};

class GameState : public State
{
private:
	PlatformPlayer* m_pPlayer;
	std::vector<SDL_FRect*> m_pPlatforms;
	Level* m_pLevel;

	int m_MapDamageCounter;
	const int MAPDAMAGECD = 20;//equal to FPS

public:
	GameState(Level* newLevel);
	void Update();
	void CheckCollision();
	void Render();
	void Enter();
	void Exit();
	void Resume();
};

class ClickState : public State
{
public:
	ClickState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Background* c_label;
};

class TitleState : public State
{
public:
	TitleState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Button* m_playBtn;
	Background* t_background;
};

class EndState : public State
{
public:
	EndState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Button* m_restartBtn;
	Button* m_exitBtn;
	Background* e_background;
};

#endif