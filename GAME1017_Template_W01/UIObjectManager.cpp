#include "UIObjectManager.h"
#include "TextureManager.h"


std::vector<UIObject*> UIObjectManager::UIObjectVec;

UIObjectManager::UIObjectManager()
{
	
}

UIObjectManager::~UIObjectManager()
{
	
}


void UIObjectManager::CreateSoulBar(SDL_FRect d, SDL_Renderer* r, PlatformPlayer* hero)
{
	SoulBarBorder* Tempborder = new SoulBarBorder({ 0,0,64,32 }, d, r, TextureManager::GetTexture("SoulBarBorder"));
	UIObjectVec.push_back(Tempborder);
	d.x += 64.0f;
	d.y += 0.0f;
	SoulBarFiller* Tempfiller = new SoulBarFiller({ 0,0,64,32 }, d, r, TextureManager::GetTexture("SoulBarFiller"), hero);
	UIObjectVec.push_back(Tempfiller);
	Tempborder = nullptr;
	Tempfiller = nullptr;
}

void UIObjectManager::UIUpdate()
{
	for(int i=0;i<(int)UIObjectVec.size();i++)
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
	for (int i = 0; i < (int)UIObjectVec.size(); i++)
	{
		if (UIObjectVec[i]->getActive() == false)
		{
			SDL_DestroyTexture(UIObjectVec[i]->getTex());
			delete UIObjectVec[i];
			UIObjectVec[i] = nullptr;
		}
	}
	if (!UIObjectVec.empty())
	{
		UIObjectVec.erase(remove(UIObjectVec.begin(), UIObjectVec.end(), nullptr), UIObjectVec.end());
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


