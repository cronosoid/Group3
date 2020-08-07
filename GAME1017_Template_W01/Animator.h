#pragma once

#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#include<SDL_image.h>
#include <string>
#include <map>
#include <iostream>
#include <vector>

#include "Entity.h"

// Animation engine by Maxim Dobrivskiy

class Animation
{
public: // Methods
	Animation(Uint32 maxFrames, Uint32 priority, Uint32 moveX, Uint32 moveY = 0, Uint32 startX = 0, Uint32 startY = 0, Uint32 framesFrequency = 8);
	//priority - animation with the highest priority will be played in the end of the frame
	//framesFrequency - how often the animation will change its` picture
	//Move x - how much pixels on the right should be skipped to reach the next frame
	//Move y - if the animation is face-dependent, means animation facing the right is not the same as the animation facing the left,
	//then it will move on moveY pixels downward for the left face animations, and not move downward for the right face animations
	Uint32 getMoveX() { return moveX; }
	Uint32 getMoveY() { return moveY; }
	Uint32 getStartX() { return startX; }
	Uint32 getStartY() { return startY; }
	Uint32 getPriotity() { return priority; }
	Uint32 getMaxFrames() { return maxFrames; }
	Uint32 getFramesFrequency() { return framesFrequency; }
	std::string getName() { return name; }
	void setName(std::string name) { this->name = name; }
private: // Variables
	Uint32 maxFrames;
	Uint32 moveX;
	Uint32 moveY;
	Uint32 startX;
	Uint32 startY;
	Uint32 priority;
	Uint32 framesFrequency;
	std::string name;
};

class AnimRecord
{
public:
	AnimRecord(Animation* animation);
	Animation* animation;
	Uint32 curFrame;
	Uint32 curTick;
};

class Animator
{
public: // Variables
	std::map<std::string, Animation*> animationsMap;
public: // Methods
	Animator(Entity* Player);
	Uint32 getFace() { return faceSide; }
	void setFace(Uint32 face) { face == 0 ? faceSide = 0 : faceSide = 1; }//Setting the face. 0 is right, 1 is left
	void setNextAnimation(const std::string& type);//Make a request to play an animation with specific name in the end of the frame
	void playFullAnimation(const std::string& type);
	void playAnimation();//Plays the animation with the highest priority in the "hill"
	void addAnimation(const std::string& key, Uint32 maxFrames, Uint32 priority, Uint32 moveX, Uint32 moveY = 0, Uint32 startX = 0, Uint32 startY = 0, Uint32 framesFrequency = 8);
	Animation* GetAnimation(const std::string& key);
	//Adding animation in the animations map, which can be easily played by calling setNextAnimation(*animation name*)
	void update();
	
private: // Variables
	Uint32 faceSide;
	Uint32 animFrame;
	Uint32 maxAnimationFrames;
	Uint32 curAnimationPriority;
	Uint32 lastFrameTime;
	std::string nextAnimation;
	Entity* entity;
	std::string curAnimType;

	std::vector<AnimRecord*> animRecords;
};

#endif