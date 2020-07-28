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
	static int m_score;
	static int m_time;
	static int m_currentLevel;
	double m_start, m_end;

public:
	GameState(Level* newLevel);
	void Update();
	void CheckCollision();
	void Render();
	void Enter();
	void Exit();
	void Resume();

public:
	friend class CongratulationState;
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
	Button* m_helpBtn;
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
	int m_score,m_time;
};

class CongratulationState:public State
{
public:
	CongratulationState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Button* m_nextLevelBtn,*m_exitBtn;
	Background* e_background;
	Sprite* m_Congratulation;
	Label* m_scoreLabel, * m_timeLabel;
};

class HelpState: public State
{
public:
	HelpState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Button* m_backBtn;
	Background* t_background;
	std::vector<Label*> textLabelVec;
};

class PauseHelpState : public State
{
public:
	PauseHelpState();
	void Update();
	void Render();
	void Enter();
	void Exit();
private:
	Button* m_backPauBtn;
	Button* m_helpBtn;
	Button* m_menuBtn;
	Background* t_background;
	std::vector<Label*> textLabelVec;
};

#endif