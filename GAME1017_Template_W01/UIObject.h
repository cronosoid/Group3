#pragma once
#include <string>

#include"Sprite.h"

class UIObject:public Sprite
{
public:
	UIObject(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t):Sprite(s,d,r,t)
	{
		isDisplay = true;
	}
	virtual void Update() = 0;
	virtual void Render() = 0;
	void setIsDisplay(bool a) { isDisplay = a; }
	void setIsActive(bool a) { isActive = a; }
	bool getActive() { return isActive; }
	SDL_Texture* getTex() { return m_pText; };

protected:
	bool isDisplay,
	isActive;
	std::string UIType;
};
