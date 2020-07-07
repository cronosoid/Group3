#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#define NUMENEMIES 2
#define NUMPLATFORMS 5

#include <SDL.h>
#include "Button.h"
#include "PlatformPlayer.h"
#include "Label.h"
#include "Animator.h"
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
	Animator* m_pPlayerAnimator;
	Label* m_pCDLabel;
	int m_CDTimer{};
	double m_CDTime;
	//std::string CDText{"Fireball Ready!"};
	std::vector<SDL_FRect*> m_pPlatforms;
	
public:
	GameState();
	void Update();
	void CheckCollision();
	void Render();
	void Enter();
	void Exit();
	void Resume();
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