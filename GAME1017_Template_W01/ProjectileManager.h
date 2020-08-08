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
	void Update();
	static vector<Projectile*> m_vProjectiles;
private:
	
};

typedef ProjectileManager PMA;