#include "NoemalEnemy.h"

#include"ActionPart.h"

#include"Pause.h"
#include"EditMode.h"

NoemalEnemy::NoemalEnemy(const MelLib::Vector3& pos) :Enemy(pos,"")
{
	moveSpeed = 0.2f;
}

void NoemalEnemy::Update()
{

	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

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
