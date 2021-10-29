#include "NoemalEnemy.h"

NoemalEnemy::NoemalEnemy(const MelLib::Vector3& pos) :Enemy(pos,"")
{

}

void NoemalEnemy::Update()
{
	CalcPlayerRoute();
	AddPosition(routeVectors[0] * 0.2f);
}

void NoemalEnemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
}
