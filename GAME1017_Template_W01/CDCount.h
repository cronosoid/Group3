#pragma once
#include "Label.h"
#include "PlatformPlayer.h"

class CDCount
{
public:
	CDCount(PlatformPlayer* hero);
	void Update();
	void Render();
private:
	Label* CDLabel;
	PlatformPlayer* hero;
	std::string CDText{};
};
