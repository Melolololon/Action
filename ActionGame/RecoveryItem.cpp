#include "RecoveryItem.h"

RecoveryItem::RecoveryItem(const MelLib::Vector3& pos):
	GameItem(pos)
{
}

void RecoveryItem::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if(CheckHitPlayer(object))
	{
		// ‰ñ•œ
		pPlayer->LifeUp(RECOVERY_HP);
		
		// íœ
		Erase();
	}
}
