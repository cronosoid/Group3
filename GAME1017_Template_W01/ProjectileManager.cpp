#include "ProjectileManager.h"
#include "MoveManager.h"

void ProjectileManager::Update()
{
	for (auto projectile = PMA::Instance().GetProjectiles().begin(); projectile != PMA::Instance().GetProjectiles().end();)
	{
		(*projectile)->Update();
		if ((*projectile)->GetDstP()->x + (*projectile)->GetDstP()->w < -MOMA::getTotalMove().x or (*projectile)->GetDstP()->x > MOMA::getRightBorder())
		{
			std::cout << "Proj deleted\n";
			delete* projectile;
			projectile = PMA::Instance().GetProjectiles().erase(projectile);
		}
		else
		{
			projectile++;
		}
	}
}