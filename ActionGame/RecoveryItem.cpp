#include "RecoveryItem.h"

RecoveryItem::RecoveryItem(const MelLib::Vector3& pos):
	GameItem(pos)
{
}

void RecoveryItem::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if(CheckHitPlayer(object))
	{
		// ��
		pPlayer->LifeUp(RECOVERY_HP);
		
		// �폜
		Erase();
	}
}
