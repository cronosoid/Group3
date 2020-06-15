#pragma once
#include <vector>
#include "Projectile.h"

using namespace std;

class ProjectileManager
{
public:
	vector<Projectile*>& GetProjectiles()
	{
		return m_vProjectiles;
	}

	static ProjectileManager& Instance()
	{
		static ProjectileManager instance;
		return instance;
	}
private:
	vector<Projectile*> m_vProjectiles;
};

typedef ProjectileManager ProMA;