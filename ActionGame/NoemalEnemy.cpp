#include "NoemalEnemy.h"

#include"ActionPart.h"

#include"Pause.h"
#include"EditMode.h"

#include"EnemyAttack.h"

#include<GameObjectManager.h>
#include<LibMath.h>


void NoemalEnemy::LoadResource()
{
	MelLib::ModelData::Load("Resources/Model/Enemy/Mokuzin/Mokuzin_03.fbx", true, "NormalEnemy");
}

NoemalEnemy::NoemalEnemy(const MelLib::Vector3& pos, int wallNum) :
	Enemy(pos, 3, 3.0f,wallNum, "NormalEnemy")
{
	capsuleDatas.resize(1);
	capsuleDatas[0].SetRadius(4.5f);
	capsuleDatas[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, -10.0f, 0)));

	segment3DDatas.resize(1);
	segment3DDatas[0].SetPosition(capsuleDatas[0].GetSegment3DData().GetPosition());


	modelObjects["main"].SetScale(3);




	modelObjects["main"].SetAngle(MelLib::Vector3(0, 0, 0));
	modelObjects["main"].SetAnimationPlayFlag(true);

	modelObjects["main"].Update();

}

void NoemalEnemy::Update()
{

	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;
	

	modelObjects["main"].Update();

	// 死んでたらreturn
	if (isDead)
	{
		// カプセルの判定処理を無視するように
		collisionFlag.capsule = false;
		Dead();
		return;
	}

	// 無敵の時間進める
	CheckMutekiEnd();
	
	// 硬直中だったらreturn
	if (isStun)
	{
		// 硬直関係の処理
		Stun();
		return;
	}

	// ここに攻撃条件を記述
	if (CheckPlayerDistance(13.0f) && !isAttack)
	{
		AttackStart();
	}
	else if(!isAttack)
	{
		if(CalcPlayerRoute())modelObjects["main"].SetAnimation("Move");
		else modelObjects["main"].SetAnimation("No_Move");
		modelObjects["main"].SetAnimationEndStopFlag(false);
	}
	if(isAttack)
	{
		//modelObjects["main"].SetAnimation("Attack");

		if(modelObjects["main"].GetAnimationEndFlag())
		{
			isAttack = false;
		}
	}

	CheckAttackEnd();

	// 回転
	RotModel();

	// 時間になったら攻撃
	if (attackTimer.GetNowTime() >= ATTACK_START_TIME)
	{
		// 攻撃判定の追加
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
			(
				40,
				MelLib::Vector3(-12, 17, 0),
				3.0f,
				60 * 0.2,
				modelObjects["main"],
				MelLib::Vector3(0, 0, 0),
				MelLib::Vector3(0, 0, 0),
				3
				)
		);

		attackTimer.ResetTimeZero();
		attackTimer.SetStopFlag(true);
	}


	//modelObjects["main"].SetAnimation("No_Move");

	// 固定
	//modelObjects["main"].SetAnimation("Test");
	


	// 浮き防止
	FallStart(0.0f);
	CalcMovePhysics();

	//modelObjects["main"].SetAnimation("_T");
}

//void NoemalEnemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
//{
//}
