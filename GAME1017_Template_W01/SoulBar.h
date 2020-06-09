#pragma once
#include "UIObject.h"
#include"PlatformPlayer.h"

class SoulBarBorder:public UIObject
{
public:
	SoulBarBorder(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t):UIObject(s,d,r,t)
	{
		isDisplay = true;
		UIType = "SoulBarBorder";
	}
	virtual void Update() override;
	virtual void Render() override;

private:

};

class SoulBarFiller:public UIObject
{
public:
	SoulBarFiller(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, PlatformPlayer* hero) :UIObject(s, d, r, t)
	{
		isDisplay = true;
		this->hero = hero;
		UIType = "SoulBarFiller";
	}
	virtual void Update() override;
	virtual void Render() override;

	PlatformPlayer* hero;
private:
	double HeaPercent;

	//PlatformPlayer* hero;
};
