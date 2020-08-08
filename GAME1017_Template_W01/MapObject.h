#pragma once
#include "Sprite.h"


class MapObject :public Sprite
{
public:

protected:
	bool m_isActive,
		m_IsHurt,
		m_CanCollide,
		m_CanMove,
		m_mDir;//true:to end false: to start
	int m_Damage;
	float m_moveSpeed;
  std::string type;//used as tag for different map objects
	SDL_FPoint m_startPoint, m_endPoint;
public:
	MapObject(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	MapObject(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, SDL_FPoint start, SDL_FPoint end);
	//make sure x and y of start equal to x and y of d(Destination Rectangle);
	//make sure start and end are on a line
	virtual void Update() = 0;
	virtual void Render() = 0;

	bool getActive();
	bool getIsHurt();
	bool getCanCollide();
	bool getCanMove();
	int getDamage();
	void changeDir();
	void moveStart(double x,double y);
	void moveEnd(double x,double y);
	std::string getType() { return type; };
	void moveMapObject();
protected:

};

class Plate :public MapObject
{
public:

private:
	const int MOVESPEED = 2.0;
public:
	Plate(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	Plate(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, SDL_FPoint start, SDL_FPoint end);
	void Update() override;
	void Render() override;
	
private:

};

class Spike :public MapObject
{
public:

private:
	const int MOVESPEED = 7.5;
public:
	Spike(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	Spike(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, SDL_FPoint start, SDL_FPoint end);
	void Update() override;
	void Render() override;
private:

};

class Portal: public MapObject
{
public:
	Portal(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	void Update() override;
	void Render() override;
	
private:

};

class UnstableBrick:public MapObject
{
public:

private:
	const int MOVESPEED = 2.0;
	const int FALLSPEED = 5.0;
	const int BREAKTIME = 20;
	int m_brokenTime;
	int m_status;//1:normal 2:breaking
public:
	UnstableBrick(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	UnstableBrick(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, SDL_FPoint start, SDL_FPoint end);
	void Update() override;
	void Render() override;
	void changeStatus(int state);
	int getStatus();
private:

};