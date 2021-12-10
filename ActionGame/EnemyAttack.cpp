#include "EnemyAttack.h"

EnemyAttack::EnemyAttack(const MelLib::Vector3& pos,const float radius,const int deadTime)
{
	sphereData.resize(1);
	sphereData[0].SetPosition(pos);
	sphereData[0].SetRadius(radius);

	SetPosition(pos);

	deadTimer.SetMaxTime(deadTime);
	deadTimer.SetStopFlag(false);
}

void EnemyAttack::Update()
{
	if(deadTimer.GetMaxOverFlag())
	{
		eraseManager = true;
	}
}

void EnemyAttack::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{

}
