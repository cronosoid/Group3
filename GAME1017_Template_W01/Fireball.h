#include "Projectile.h"

class Fireball : public Projectile
{
protected:
	PlatformPlayer* hero;
	std::vector<Enemies*> EnemiesVec;
	std::vector<MapObject*> mapObjects;
public:
	Fireball(PlatformPlayer* hero, std::vector<Enemies*> EnemiesVec, std::vector<MapObject*> mapObjects, SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t,  double spd, int face, double damage = 0,
		int framesFrequency = 0, int maxFrames = 0, int m_moveX = 0, int m_moveY = 0);
    void Update() override;
};