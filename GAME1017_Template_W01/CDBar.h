#pragma once
#include "UIObject.h"
#include "PlatformPlayer.h"

class CDBorder :public UIObject
{
public:
	CDBorder(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :UIObject(s, d, r, t)
	{
		isDisplay = true;
		UIType = "SoulBarBorder";
	}
	virtual void Update() override;
	virtual void Render() override;

private:
	int animFrame;
	int maxAnimFrame;
	double animFramerate;
	Uint32 changeTime;
};

class CDFiller :public UIObject
{
public:
	CDFiller(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, PlatformPlayer* hero) :UIObject(s, d, r, t)
	{
		maxWidth = d.w;
		isDisplay = true;
		this->hero = hero;
		UIType = "SoulBarFiller";
	}
	virtual void Update() override;
	virtual void Render() override;

	PlatformPlayer* hero;
private:
	double HeaPercent;
	double maxWidth;

	//PlatformPlayer* hero;
};