#include "NormalEnemyAttack.h"

Player* NormalEnemyAttack::pPlayer;

NormalEnemyAttack::NormalEnemyAttack
(
	unsigned int power,
	float radius
)
	:power(power)
{
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetRadius(radius);


}


void NormalEnemyAttack::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if(typeid(object) == typeid(Player)
		&& !pPlayer->GetIsMuteki())
	{
		pPlayer->DownHP(power);
		eraseManager = true;
	}
}
