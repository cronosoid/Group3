#include <iostream>
#include "Background.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "StateManager.h"
#include <SDL.h>

Background::Background(SDL_Rect src, SDL_FRect dst, SDL_Renderer* r, SDL_Texture* t) : Sprite(src, dst, r, t) {} // Used to create the background in the title state.
