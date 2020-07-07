#pragma once
#include "MapObjectManager.h"
#include "PlatformPlayer.h"

class Level
{
public:
	Level();
	~Level() {};

	virtual void Load(PlatformPlayer* Player) = 0;
	virtual void Update() = 0;
	int GetLevelWidth() { return m_levelWidth; }
	int GetLevelHight() { return m_levelHight; }
protected:
	int m_levelWidth, m_levelHight;
	MapObject* m_pPortal;
	PlatformPlayer* m_pPlayer;
};

class Level1 : public Level
{
public:
	Level1();
	~Level1() {};

	void Load(PlatformPlayer* Player) override;
	void Update() override;

private:

};

class Level2 : public Level
{
public:
	Level2();
	~Level2() {};

	void Load(PlatformPlayer* Player) override;
	void Update() override;

private:

};