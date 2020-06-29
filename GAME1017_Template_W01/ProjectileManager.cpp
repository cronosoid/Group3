#include "ProjectileManager.h"
#include "MoveManager.h"

void ProjectileManager::Update()
{
	for (auto projectile = PMA::Instance().GetProjectiles().begin(); projectile != PMA::Instance().GetProjectiles().end();)
	{
		(*projectile)->Update();
		if ((*projectile)->GetDstP()->x + (*projectile)->GetDstP()->w < 0 or (*projectile)->GetDstP()->x > MOMA::getRightBorder())
		{
			delete* projectile;
			projectile = PMA::Instance().GetProjectiles().erase(projectile);
		}
		else
		{
			projectile++;
		}
	}
}