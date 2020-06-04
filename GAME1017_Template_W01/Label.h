#pragma once
#ifndef _LABEL_H_
#define _LABEL_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Label
{
private:
	TTF_Font* m_Font;
	SDL_Color m_TextColor;
	SDL_FRect m_rTextRect;
	SDL_Texture* m_pTexture;
	char m_String[256];
public:
	Label(std::string key, const float x, const float y, const char* str, const SDL_Color col = { 255,255,255,255 });
	~Label();
	void Render();
	void SetText(const char* c);
	void SetPos(const float x, const float y);
	void SetColor(const SDL_Color& col);
	void UseFont(std::string key);
};

#endif