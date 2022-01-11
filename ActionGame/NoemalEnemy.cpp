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

NoemalEnemy::NoemalEnemy(const MelLib::Vector3& pos) :
	Enemy(pos, 3, 3.0f, "NormalEnemy")
{
	capsuleData.resize(1);
	capsuleData[0].SetRadius(5.0f);
	capsuleData[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, -10.0f, 0)));

	segment3DData.resize(1);
	segment3DData[0].SetPosition(capsuleData[0].GetSegment3DData().GetPosition());


	modelObjects["main"].SetScale(3);




	modelObjects["main"].SetAngle(MelLib::Vector3(0, 0, 0));
	modelObjects["main"].SetAnimationPlayFlag(true);

	modelObjects["main"].Update();

	// ��
	
	/*MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
		(
			3,
			MelLib::Vector3(-12, 17, 0),
			3.0f,
			60 * 15,
			modelObjects["main"],
			MelLib::Vector3(0, 0, 0),
			MelLib::Vector3(0, 0, 0),
			3
			)
	);*/
}

void NoemalEnemy::Update()
{

	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;
	

	modelObjects["main"].Update();

	// ����ł���return
	if (isDead)
	{
		Dead();
		return;
	}

	// ���G�̎��Ԑi�߂�
	CheckMutekiEnd();
	
	// �d������������return
	if (isStun)
	{
		// �d���֌W�̏���
		Stun();
		return;
	}

	// �����ɍU���������L�q
	if (CheckPlayerDistance(15.0f) && !isAttack)
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

	// ��]
	RotModel();

	// ���ԂɂȂ�����U��
	if (attackTimer.GetNowTime() == ATTACK_START_TIME)
	{
		// �U������̒ǉ�
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
			(
				3,
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

	// �Œ�
	//modelObjects["main"].SetAnimation("Test");
	


	// �����h�~
	FallStart(0.0f);
	CalcMovePhysics();

	//modelObjects["main"].SetAnimation("_T");
}

//void NoemalEnemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
//{
//}
