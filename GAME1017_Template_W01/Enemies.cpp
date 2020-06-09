#include "Enemies.h"
#include <algorithm>
#include <iostream>
#include "Engine.h"

Enemies::Enemies(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t)
	:Entity(s, d, r, t)
{
	isActive = true;
	isAlive = true;
}

void Enemies::setActive(bool a) { isActive = a; }
bool Enemies::getActive() { return isActive; }
bool Enemies::getAlive() { return isAlive; }
void Enemies::setAlive(bool a) { isActive = a; }
SDL_Texture* Enemies::getTex() { return m_pText; }
std::string Enemies::getType() { return enemyType; }


void Enemies::damage(int attack)
{
	health -= attack;
}
