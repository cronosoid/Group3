#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Label.h"

// Do not include any macros, initialized properties or full function definitions in this header.
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768

class Engine
{
private: // Private properties.
	bool m_running; // Loop control flag.
	bool m_pBNull;
	Uint32 m_start, m_end, m_delta, m_fps; // Fixed timestep variables.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer; // This represents the buffer to draw to.
	// Example-specific properties.

private: // Private methods.
	Engine();
	bool Init(const char* title, int xpos, int ypos, int width, int height, int flags);
	void Wake();
	void Sleep();
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
public: // Public methods.
	int Run();
	static Engine& Instance(); // This static method creates the static instance that can be accessed 'globally'
	SDL_Renderer* GetRenderer();
	SDL_Window* GetWindow() { return m_pWindow; }
	bool& Running();

	bool& isNull();
	void setNull();

	void setRunning(bool);
};

#endif