#pragma once
#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "SDL.h"

class Sprite // Inline class.
{
public: // Inherited and public.
	Sprite(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t)
		:m_src(s), m_dst(d), m_pRend(r), m_pText(t), m_angle(0.0) {}
	virtual void Render() {	SDL_RenderCopyExF(m_pRend, m_pText, GetSrcP(), GetDstP(), m_angle, 0, SDL_FLIP_NONE); }
	SDL_Rect* GetSrcP() { return &m_src; }	//where to get the sprite from
	SDL_FRect* GetDstP() { return &m_dst; }	//where the sprite will be drawn
	double& GetAngle() { return m_angle; }	//get the angle of the sprite
	void SetAngle(double a) { m_angle = a; }//seting the angle of the sprite
protected: // Private BUT inherited.
	double m_angle;
	SDL_Rect m_src;
	SDL_FRect m_dst;
	SDL_Renderer* m_pRend;
	SDL_Texture* m_pText;
private: // Private NOT inherited.
};

#endif