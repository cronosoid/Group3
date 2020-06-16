#include "UIObjectManager.h"
#include "TextureManager.h"


std::vector<UIObject*> UIObjectManager::UIObjectVec;

UIObjectManager::UIObjectManager()
{

}

UIObjectManager::~UIObjectManager()
{

}

void UIObjectManager::Init()
{
	TEMA::RegisterTexture("../Spritesheets/Soul_Bar.png", "SoulBarBorder");
	TEMA::RegisterTexture("../Spritesheets/fitBar.png", "SoulBarFiller");
}



void UIObjectManager::CreateSoulBar(SDL_FRect d1, SDL_FRect d2, SDL_Renderer* r, PlatformPlayer* hero)
{
	SoulBarBorder* Tempborder = new SoulBarBorder({ 0,0,64,32 }, d1, r, TextureManager::GetTexture("SoulBarBorder"));
	UIObjectVec.push_back(Tempborder);
	SoulBarFiller* Tempfiller = new SoulBarFiller({ 0,0,64,32 }, d2, r, TextureManager::GetTexture("SoulBarFiller"), hero);
	UIObjectVec.push_back(Tempfiller);
	Tempborder = nullptr;
	Tempfiller = nullptr;
}

void UIObjectManager::UIUpdate()
{
	for (int i = 0; i < (int)UIObjectVec.size(); i++)
	{
		UIObjectVec[i]->Update();
	}
}

void UIObjectManager::UIRender()
{
	{
		for (int i = 0; i < (int)UIObjectVec.size(); i++)
		{
			UIObjectVec[i]->Render();
		}
	}
}

void UIObjectManager::DestroyUIObjects()
{
	for (auto uiobj = UIObjectVec.begin(); uiobj != UIObjectVec.end(); )
	{
		if (not (*uiobj)->getActive())
		{
			delete *uiobj;
			uiobj = UIObjectVec.erase(uiobj);
		}
		else
		{
			++uiobj;
		}
	}
}

void UIObjectManager::setHiden(UIObject* UIAsset)
{
	UIAsset->setIsDisplay(false);
}

void UIObjectManager::setActive(UIObject* UIAsset)
{
	UIAsset->setIsActive(false);
}


