#include "ProjectileManager.h"
#include "MoveManager.h"

std::vector<Projectile*> ProjectileManager::m_vProjectiles;
void ProjectileManager::Update()
{
	for (auto projectile = PMA::Instance().GetProjectiles().begin(); projectile != PMA::Instance().GetProjectiles().end();)
	{
		(*projectile)->Update();
		if ((*projectile)->GetDstP()->x + (*projectile)->GetDstP()->w < -MOMA::GetTotalMove().x or (*projectile)->GetDstP()->x > MOMA::GetRightBorder())
		{
			delete* projectile;
			projectile = PMA::Instance().GetProjectiles().erase(projectile);
		}
		else if ((*projectile)->getIsActive() == false)
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