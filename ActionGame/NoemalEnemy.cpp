#include "NoemalEnemy.h"

#include"ActionPart.h"

#include"Pause.h"
#include"EditMode.h"

NoemalEnemy::NoemalEnemy(const MelLib::Vector3& pos) :
	Enemy(pos, 3, 0.2f, 60 * 0.7, "")
{
	capsuleData.resize(1);
	capsuleData[0].SetRadius(10.0f);
	capsuleData[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0,25.0f,0), pos + MelLib::Vector3(0, -25.0f, 0)));


	attackTimer.SetMaxTime(60 * 3);

}

void NoemalEnemy::Update()
{
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;
	
	if(attackTimer.GetMaxOverFlag())
	{
		isAttack = false;
		attackTimer.ResetTimeZero();
		attackTimer.SetStopFlag(true);
	}

	if (CheckPlayerDistance(3.0f) || isAttack) 
	{
		isAttack = true;
		attackTimer.SetStopFlag(false);
		//Attack();
	}
	else 
	{
		CalcPlayerRoute();
	}//AddPosition(routeVectors[0] * 0.2f);


	CheckMutekiEnd();
}

//void NoemalEnemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
//{
//}
