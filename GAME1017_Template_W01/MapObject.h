#pragma once
#ifndef _MAPOBJECT_H_
#define _MAPOBJECT_H_

#include "Sprite.h"

class MapObject :public Sprite
{
public:

protected:
	bool isHurt,
		canCollide;
public:
	MapObject(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	virtual void Update() = 0;
	virtual void Render() = 0;

	bool getCollision() { return canCollide; }

	SDL_FRect* getFRect();
protected:

};

class Plate :public MapObject
{
public:

private:

public:
	Plate(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	void Update() override;
	void Render() override;
private:

};

class Spike :public MapObject
{
public:

private:

public:
	Spike(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t);
	void Update() override;
	void Render() override;
private:

};
#endif