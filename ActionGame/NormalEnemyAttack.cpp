#include "NormalEnemyAttack.h"

Player* NormalEnemyAttack::pPlayer;

NormalEnemyAttack::NormalEnemyAttack
(
	unsigned int power,
	float radius
)
	:power(power)
{
	sphereData.resize(1);
	sphereData[0].SetRadius(radius);


}


void NormalEnemyAttack::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
	if(typeid(*object) == typeid(Player))
	{
		pPlayer->DownHP(power);
	}
}
