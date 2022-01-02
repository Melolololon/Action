#include "RecoveryItem.h"

RecoveryItem::RecoveryItem(const MelLib::Vector3& pos):
	GameItem(pos)
{
}

void RecoveryItem::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
	if(CheckHitPlayer(*object))
	{
		// ��
		pPlayer->LifeUp(RECOVERY_HP);
		
		// �폜
		Erase();
	}
}
