#include "Animator.h"

Animator::Animator(PlatformPlayer* Player)
{
	this->Player = Player;
	this->curAnimationPriority = 0;
	this->faceSide = 0;
	this->animFrame = 0;
	this->maxAnimationFrames = 0;
	this->lastFrameTime = 0;
}

void Animator::setNextAnimation(const std::string& type)
{
	Animation* anim = animationsMap[type];
	Animation* nextAnim = animationsMap[nextAnimation];
	if ((nextAnim == nullptr) or (anim->getPriotity() >= nextAnim->getPriotity()))
	{
		nextAnimation = type;
	}
}

void Animator::playAnimation()
{
	if (animationsMap[nextAnimation] != nullptr)
	{
		//std::cout << "Playing " << nextAnimation << std::endl;
		Animation* anim = animationsMap[nextAnimation];
		if (curAnimType != nextAnimation)
		{
			curAnimationPriority = anim->getPriotity();
			animFrame = 0;
			lastFrameTime = 0;
			curAnimType = nextAnimation;
			maxAnimationFrames = anim->getMaxFrames();
		}
		if (SDL_GetTicks() - lastFrameTime >= anim->getFramesFrequency())
		{
			lastFrameTime = SDL_GetTicks();
			Player->SetSrcCords(anim->getStartX() + anim->getMoveX() * animFrame, anim->getStartY() + anim->getMoveY() * faceSide);
			if (++animFrame >= maxAnimationFrames)
			{
				animFrame = 0;
			}
		}
	}
	nextAnimation = "";
}

void Animator::addAnimation(const std::string& key, Uint32 maxFrames, Uint32 priority, Uint32 moveX, Uint32 moveY, Uint32 startX
	, Uint32 startY, Uint32 framesFrequency)
{
	animationsMap[key] = new Animation(maxFrames, priority, moveX, moveY, startX, startY);
}

Animation::Animation(Uint32 maxFrames, Uint32 priority, Uint32 moveX, Uint32 moveY, Uint32 startX, Uint32 startY, Uint32 framesFrequency)
{
	this->startX = startX;
	this->startY = startY;
	this->maxFrames = maxFrames;
	this->moveX = moveX;
	this->moveY = moveY;
	this->priority = priority;
	this->framesFrequency = framesFrequency*10;
}
