#pragma once
#ifndef _DEBUGMANAGER_H_
#define _DEBUGMANAGER_H_

#include <vector>
#include "SDL.h"

class DebugManager
{
public:
	static int s_debugMode;
	static void DrawLine(const SDL_Point start, const SDL_Point end, const SDL_Color col);
	static void QueueLine(const SDL_Point start, const SDL_Point end, const SDL_Color col);
	static void FlushLines();
	static void Quit();
private:
	static std::vector<SDL_Point> s_points;
	static std::vector<SDL_Color> s_colors;
private:
	DebugManager() {}
};

typedef DebugManager DEMA;

#endif