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
	Enemy(pos, 3, 0.2f, "NormalEnemy")
{
	capsuleData.resize(1);
	capsuleData[0].SetRadius(10.0f);
	capsuleData[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, -25.0f, 0)));


	attackTimer.SetMaxTime(60 * 2);

	modelObjects["main"].SetScale(3);




	modelObjects["main"].SetAngle(MelLib::Vector3(0, 0, 0));
	modelObjects["main"].SetAnimationPlayFlag(true);

	modelObjects["main"].Update();

	// ��
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
		(
			3,
			MelLib::Vector3(12,17,0),
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
	// �����ɍU���������L�q
	if (CheckPlayerDistance(20.0f))
	{
		//AttackStart();
	}
	else
	{
		//CalcPlayerRoute();

		modelObjects["main"].SetAnimation("Move");
	}
	CheckAttackEnd();

	// ��]
	//RotModel();

	// ���ԂɂȂ�����U��
	if (attackTimer.GetNowTime() == ATTACK_START_TIME)
	{
		// �U������̒ǉ�
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

	modelObjects["main"].SetAnimation("No_Move");

	// �Œ�
	//modelObjects["main"].SetAnimation("Test");
}

//void NoemalEnemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
//{
//}
