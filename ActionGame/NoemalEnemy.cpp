#include "NoemalEnemy.h"

#include"ActionPart.h"

#include"Pause.h"
#include"EditMode.h"

NoemalEnemy::NoemalEnemy(const MelLib::Vector3& pos) :
	Enemy(pos, 3, 0.2f, "")
{
	capsuleData.resize(1);
	capsuleData[0].SetRadius(1.0f);
	capsuleData[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0,1.0f,0), pos + MelLib::Vector3(0, -1.0f, 0)));
}

void NoemalEnemy::Update()
{
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

	CalcPlayerRoute();
	//AddPosition(routeVectors[0] * 0.2f);

	Attack();

	CheckMutekiEnd();
}

//void NoemalEnemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
//{
//}

void NoemalEnemy::Attack()
{

}
