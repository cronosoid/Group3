#include "CDCount.h"

CDCount::CDCount(PlatformPlayer* hero)
{
	CDLabel = new Label("font", 0, 0, CDText.c_str());
	this->hero = hero;
	std::cout << "label init!" << std::endl;
}

void CDCount::Update()
{
	CDText = std::to_string((1000 - (SDL_GetTicks() - hero->getMagicTime())) / 1000);
	CDLabel->SetText(CDText.c_str());
}

void CDCount::Render()
{
	CDLabel->Render();
}

