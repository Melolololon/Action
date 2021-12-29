#include "NoemalEnemy.h"

#include"ActionPart.h"

#include"Pause.h"
#include"EditMode.h"

#include"EnemyAttack.h"

#include<GameObjectManager.h>
#include<LibMath.h>


void NoemalEnemy::LoadResource()
{
	MelLib::ModelData::Load("Resources/Model/Enemy/Mokuzin/Mokuzin.fbx", true, "NormalEnemy");
}

NoemalEnemy::NoemalEnemy(const MelLib::Vector3& pos) :
	Enemy(pos, 3, 0.2f, "NormalEnemy")
{
	capsuleData.resize(1);
	capsuleData[0].SetRadius(10.0f);
	capsuleData[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, -25.0f, 0)));


	attackTimer.SetMaxTime(60 * 2);

	modelObjects["main"].SetScale(3);



	// 仮
 	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
		(
			1,
			MelLib::Vector3(30,30,-10),
			3.0f,
			60 * 150,
			modelObjects["main"],
			MelLib::Vector3(28, 0, 0),
			MelLib::Vector3(0, 0, 0),
			3
			)
	);
	modelObjects["main"].SetAngle(MelLib::Vector3(28, 0, 0));
}

void NoemalEnemy::Update()
{

	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;
	

	// ここに攻撃条件を記述
	if (CheckPlayerDistance(4.0f))
	{
		AttackStart();
	}
	else
	{
		//CalcPlayerRoute();
	}
	CheckAttackEnd();

	// 回転
	//RotModel();

	// 時間になったら攻撃
	if (attackTimer.GetNowTime() == ATTACK_START_TIME)
	{
		// 攻撃判定の追加
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
			(
				3,
				GetPosition(),
				3.0f,
				60 * 0.3,
				modelObjects["main"],
				0,
				MelLib::Vector3(28, 0, 0), 
				3
			)
		);

	}

	CheckMutekiEnd();

}

//void NoemalEnemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
//{
//}
