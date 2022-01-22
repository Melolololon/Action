#include "NormalEnemyAttack.h"

Player* NormalEnemyAttack::pPlayer;

NormalEnemyAttack::NormalEnemyAttack(unsigned int power, const MelLib::Vector3& attackStartPos, float radius, int deadTime, const MelLib::ModelObject& model):
	power(power)
	,MODEL(model)
{
	sphereData.resize(1);
	sphereData[0].SetRadius(radius);

	deadTimer.SetMaxTime(deadTime);
	deadTimer.SetStopFlag(false);

}

void NormalEnemyAttack::Update()
{
	if (deadTimer.GetMaxOverFlag())eraseManager = true;
}

void NormalEnemyAttack::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
	if(typeid(*object) == typeid(Player))
	{
		pPlayer->DownHP(power);
	}
}
