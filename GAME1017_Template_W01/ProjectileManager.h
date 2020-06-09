#pragma once
#include <vector>
#include "Fireball.h"

using namespace std;

class ProjectileManager
{
public:
	vector<Fireball*>& GetFireBalls()
	{
		return m_vPFireballs;
	}

	static ProjectileManager& Instance()
	{
		static ProjectileManager instance;
		return instance;
	}
private:
	vector<Fireball*> m_vPFireballs;
};

typedef ProjectileManager ProMA;