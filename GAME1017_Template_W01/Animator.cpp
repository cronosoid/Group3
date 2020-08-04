#include "Animator.h"
#include "SoundManager.h"

Animator::Animator(Entity* Player)
{
	this->entity = Player;
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

void Animator::playFullAnimation(const std::string& type)
{
	for (AnimRecord* rec : animRecords)
	{
		if (type == rec->animation->getName())
		{
			rec->curTick = 0;
			rec->curFrame = 0;
			return;
		}
	}
	std::cout << "New anim record: " << type << "\n";
	animRecords.push_back(new AnimRecord(animationsMap[type]));
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
			for (AnimRecord* rec : animRecords)
			{
				if (nextAnimation == rec->animation->getName())
				{
					animFrame = rec->curFrame;
					lastFrameTime = rec->curTick;
				}
			}
		}
		if (SDL_GetTicks() - lastFrameTime >= anim->getFramesFrequency())
		{
			lastFrameTime = SDL_GetTicks();
			if (entity == nullptr)
				std::cout << "No entity!!!!!!!!!\n";
			entity->SetSrcCords(anim->getStartX() + anim->getMoveX() * animFrame, anim->getStartY() + anim->getMoveY() * faceSide);
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
	animationsMap[key] = new Animation(maxFrames, priority, moveX, moveY, startX, startY, framesFrequency);
	animationsMap[key]->setName(key);
	
}

void Animator::update()
{
	for (auto animRec = animRecords.begin(); animRec != animRecords.end();)
	{
		bool moved = false;
		if (SDL_GetTicks() - (*animRec)->curTick >= (*animRec)->animation->getFramesFrequency())
		{
			(*animRec)->curTick = SDL_GetTicks();
			if (++(*animRec)->curFrame >= (*animRec)->animation->getMaxFrames())
			{
				delete *animRec;
				animRec = animRecords.erase(animRec);
				moved = true;
			}
		}
		if (not moved)
		{
			animRec++;
		}
	}

	Animation* nextAnim = animationsMap[nextAnimation];
	for (AnimRecord* rec : animRecords)
	{
		if ((nextAnim == nullptr) or (rec->animation->getPriotity() >= nextAnim->getPriotity()))
		{
			nextAnimation = rec->animation->getName();
		}
	}
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

AnimRecord::AnimRecord(Animation* animation)
{
	this->animation = animation;
	this->curFrame = 0;
}