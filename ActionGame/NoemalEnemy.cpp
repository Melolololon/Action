#include "NoemalEnemy.h"

#include"ActionPart.h"

NoemalEnemy::NoemalEnemy(const MelLib::Vector3& pos) :Enemy(pos,"")
{
	moveSpeed = 0.2f;
}

void NoemalEnemy::Update()
{
	if (ActionPart::GetStopFlag())return;

	CalcPlayerRoute();
	//AddPosition(routeVectors[0] * 0.2f);

	Attack();
}

void NoemalEnemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
}

void NoemalEnemy::Attack()
{

}
