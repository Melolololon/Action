#include "NormalEnemyAttack.h"

Player* NormalEnemyAttack::pPlayer;

NormalEnemyAttack::NormalEnemyAttack
(
	unsigned int power,
	float radius
)
	:power(power)
{
	sphereDatas.resize(1);
	sphereDatas[0].SetRadius(radius);


}


void NormalEnemyAttack::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
	if(typeid(*object) == typeid(Player)
		&& !pPlayer->GetIsMuteki())
	{
		pPlayer->DownHP(power);
		eraseManager = true;
	}
}
