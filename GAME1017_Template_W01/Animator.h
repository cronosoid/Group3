#pragma once

#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#include<SDL_image.h>
#include <string>
#include <map>
#include <iostream>
#include "PlatformPlayer.h"

// Animation engine by Maxim Dobrivskiy

class Animation
{
public: // Methods
	Animation(Uint32 maxFrames, Uint32 priority, Uint32 moveX, Uint32 moveY = 0, Uint32 startX = 0, Uint32 startY = 0, Uint32 framesFrequency = 8);

	Uint32 getMoveX() { return moveX; }
	Uint32 getMoveY() { return moveY; }
	Uint32 getStartX() { return startX; }
	Uint32 getStartY() { return startY; }
	Uint32 getPriotity() { return priority; }
	Uint32 getMaxFrames() { return maxFrames; }
	Uint32 getFramesFrequency() { return framesFrequency; }
private: // Variables
	Uint32 maxFrames;
	Uint32 moveX;
	Uint32 moveY;
	Uint32 startX;
	Uint32 startY;
	Uint32 priority;
	Uint32 framesFrequency;
};

class Animator
{
public: // Variables
	std::map<std::string, Animation*> animationsMap;
public: // Methods
	Animator(PlatformPlayer* Player);
	Uint32 getFace() { return faceSide; }
	void setFace(Uint32 face) { face == 0 ? faceSide = 0 : faceSide = 1; }
	void setNextAnimation(const std::string& type);
	void playAnimation();
	void addAnimation(const std::string& key, Uint32 maxFrames, Uint32 priority, Uint32 moveX, Uint32 moveY = 0, Uint32 startX = 0, Uint32 startY = 0, Uint32 framesFrequency = 8);
private: // Variables
	Uint32 faceSide;
	Uint32 animFrame;
	Uint32 maxAnimationFrames;
	Uint32 curAnimationPriority;
	Uint32 lastFrameTime;
	std::string nextAnimation;
	PlatformPlayer* Player;
	std::string curAnimType;
};

#endif