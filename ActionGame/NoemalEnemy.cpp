#include "NoemalEnemy.h"

#include"ActionPart.h"

#include"Pause.h"
#include"EditMode.h"

#include"EnemyAttack.h"

#include<GameObjectManager.h>
#include<LibMath.h>


void NoemalEnemy::LoadResource()
{
	MelLib::ModelData::Load("Resources/Model/Enemy/Mokuzin/Mokuzin_02.fbx", true, "NormalEnemy");
}

NoemalEnemy::NoemalEnemy(const MelLib::Vector3& pos) :
	Enemy(pos, 3, 0.8f, "NormalEnemy")
{
	capsuleData.resize(1);
	capsuleData[0].SetRadius(5.0f);
	capsuleData[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, -10.0f, 0)));

	segment3DData.resize(1);
	segment3DData[0].SetPosition(capsuleData[0].GetSegment3DData().GetPosition());

	attackTimer.SetMaxTime(60 * 2);

	modelObjects["main"].SetScale(3);




	modelObjects["main"].SetAngle(MelLib::Vector3(0, 0, 0));
	modelObjects["main"].SetAnimationPlayFlag(true);

	modelObjects["main"].Update();

	// ‰¼
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
		(
			3,
			MelLib::Vector3(-12,17,0),
			3.0f,
			60 * 0.3,
			modelObjects["main"],
			MelLib::Vector3(0,0,0),
			MelLib::Vector3(0, 0, 0),
			3
			)
	);
}

void NoemalEnemy::Update()
{

	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;
	

	modelObjects["main"].Update();
	// ‚±‚±‚ÉUŒ‚ðŒ‚ð‹Lq
	if (CheckPlayerDistance(10.0f) )
	{
		AttackStart();
	}
	else if(!isAttack)
	{
		CalcPlayerRoute();

		modelObjects["main"].SetAnimation("Move");
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

	// ‰ñ“]
	RotModel();

	// ŽžŠÔ‚É‚È‚Á‚½‚çUŒ‚
	if (attackTimer.GetNowTime() == ATTACK_START_TIME)
	{
		// UŒ‚”»’è‚Ì’Ç‰Á
		/*MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
			(
				3,
				GetPosition(),
				3.0f,
				60 * 0.3,
				modelObjects["main"],
				0,
				MelLib::Vector3(0, 0, 0), 
				3
			)
		);*/

	}

	CheckMutekiEnd();

	//modelObjects["main"].SetAnimation("No_Move");

	// ŒÅ’è
	//modelObjects["main"].SetAnimation("Test");
	//modelObjects["main"].SetAnimation("_T");


	// •‚‚«–hŽ~
	FallStart(0.0f);
}

//void NoemalEnemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
//{
//}
