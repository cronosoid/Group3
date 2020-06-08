﻿#include "Swordman.h"

#include <iostream>

#include "MathManager.h"
#include"TextureManager.h"
#include <SDL_image.h>

const int INIHEALTH = 100;
const int SwordmanATTACK = 10;
const int SwordmanDEFENCE = 10;
const int SwordmanATKCD = 500;
const int JUMPFORCE = 60.0;
const int GRAV = 6.0;

Swordman::Swordman(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t) :Enemies(s, d, r, t)
{
	Status = SmWaiting;
	Health = INIHEALTH;
	Attack = SwordmanATTACK;
	Defence = SwordmanDEFENCE;
	AttackCD = SwordmanATKCD;
	m_velX = m_velY = m_accelX = m_accelY = 0.0;
	m_maxVelX = 10.0;
	m_maxVelY = JUMPFORCE;
	m_grav = GRAV;
	m_drag = 0.88;
	EnemyType = "Swordman";
}

void Swordman::Update()
{
	m_src.x = m_src.y = 0;
	m_src.w = m_src.h = 640;
	//m_dst.x = 800;
	//m_dst.y = 500;
	m_dst.w = m_dst.h = 64;

	// Do X axis first.
	m_velX += m_accelX;
	m_velX *= (m_grounded ? m_drag : 1);
	m_velX = std::min(std::max(m_velX, -(m_maxVelX)), (m_maxVelX));
	m_dst.x += (int)m_velX; // Had to cast it to int to get crisp collision with side of platform.
	// Now do Y axis.
	m_velY += m_accelY + m_grav; // Adjust gravity to get slower jump.
	m_velY = std::min(std::max(m_velY, -(m_maxVelY)), (m_grav * 5));
	m_dst.y += (int)m_velY; // To remove aliasing, I made cast it to an int too.
	//std::cout <<"Swordman:"<< m_dst.x << " " << m_dst.y << " "<<isActive<<std::endl;
	m_accelX = m_accelY = 0.0;

	if (m_dst.y >= 768)
	{
		setActive(false);
	}

	if (Health <= 0)
	{
		setAlive(false);
		Status = SmDead;
	}
}

void Swordman::Render()
{
	m_pText = TextureManager::GetTexture("Swordman");

	SDL_RenderCopy(m_pRend, m_pText, &m_src, &MAMA::ConvertFRect2Rect(m_dst));
}


