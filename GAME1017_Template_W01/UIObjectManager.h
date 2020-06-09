#pragma once
#include <vector>
#include "SoulBar.h"

enum UIType
{
	soulbar = 1,
	button
};


class UIObjectManager
{
public:
	UIObjectManager();
	~UIObjectManager();

	//static void CreateUIObject(UIType type, SDL_FRect d, SDL_Renderer* r);
	static void CreateSoulBar(SDL_FRect d1, SDL_FRect d2, SDL_Renderer* r, PlatformPlayer* hero);
	static void DestroyUIObjects();
	static void setHiden(UIObject* UIAsset);
	static void setActive(UIObject* UIAsset);
	static void UIUpdate();
	static void UIRender();

	static std::vector<UIObject*> UIObjectVec;
private:
};
