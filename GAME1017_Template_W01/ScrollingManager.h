#pragma once
#ifndef _SCROLLING_MANAGER_H_
#define _SCROLLING_MANAGER_H_

#include "Background.h"

class ScrollingManager
{
public:
	static void Init();
	static void Update();
	static void Render();
	static void Clean();
	
private:
	static Background* m_backgroundArr[2];
	static Background* m_midgroundArr[2];
	static int m_scrollWidth;
	
};

typedef ScrollingManager SCMA;

#endif