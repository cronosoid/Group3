#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#define NUMPLATFORMS 5
#define NUMENEMIES 2

#include <SDL.h>
#include "Button.h"
#include "PlatformPlayer.h"
#include "Animator.h"

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
	static PlatformPlayer* m_pPlayer;
	static Animator* m_pPlayerAnimator;
	SDL_FRect* m_pPlatforms[NUMPLATFORMS];
	
public:
	GameState();
	void Update();
	void CheckCollision();
	void Render();
	void Enter();
	void Exit();
	void Resume();
	static PlatformPlayer* getPlayer() { return m_pPlayer; }
	static Animator* getPlayerAnimator() { return m_pPlayerAnimator; }
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
};

#endif