#include "Boss.h"

#include"Stage.h"
#include"LibMath.h"

#include"BossAttack.h"

#include<GameObjectManager.h>

Player* Boss::pPlayer;

void Boss::Move()
{
	static const float MOVE_SPEED = 0.2f;
	
	// 近くに移動
	MelLib::Vector3 playerNormalizeVector = pPlayer->CalcPlayerVector(GetPosition());
	MelLib::Vector3 moveVector = MelLib::Vector3(playerNormalizeVector.x, 0, playerNormalizeVector.z) * MOVE_SPEED;
	AddPosition(moveVector);
}

void Boss::SelectAction()
{
	// 攻撃中だったらスキップ
	if (currentAttack != Boss::CurrentAttack::NONE)return;
	else if (actionTimer.GetMaxOverFlag())// タイマーが一定以上になったらジャンプ攻撃
	{
		actionTimer.SetStopFlag(true);

		currentAttack = Boss::CurrentAttack::JUMP;
		modelObjects["main"].SetAnimation("Attack_Jump.001");

		return;

	}
	else actionTimer.SetStopFlag(false);


	MelLib::Vector3 myPos = GetPosition();
	MelLib::Vector3 playerPos = pPlayer->GetPosition();

	float playerDir = MelLib::LibMath::CalcDistance2D(MelLib::Vector2(myPos.x, myPos.z), MelLib::Vector2(playerPos.x, playerPos.z));

	static const float MIN_DIR = 10.0f;
	
	// 遠かったら移動
	if(playerDir >= MIN_DIR)Move();
	else// そうではないなら攻撃 
	{
		
	}
	
}

void Boss::AttackUpdate()
{
	switch (currentAttack)
	{
	case Boss::CurrentAttack::NORMAL_1:
		
		break;
	case Boss::CurrentAttack::JUMP:
		JumpAttack();
		break;
	default:
		break;
	}

	// ジャンプ攻撃のとげの処理
	if (jumpAttackTimer.GetMaxOverFlag())
	{

		static const float ONE_ATTACK_MOVE_DIR = 20.0f;

		MelLib::Vector3 AttackPos = MelLib::LibMath::FloatDistanceMoveVector3
		(
			GetPosition(),
			direction,
			(jumpAttackCount + 2) * ONE_ATTACK_MOVE_DIR
		);

		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<BossAttack>(AttackPos));

		jumpAttackCount++;
		jumpAttackTimer.ResetTimeZero();

	}
	if (jumpAttackCount >= JUMP_ATTACK_MAX_COUNT)
	{
		jumpAttackCount = 0;
		jumpAttackTimer.SetStopFlag(true);
	}
}

void Boss::NormalAttack()
{
}

void Boss::JumpAttack()
{
	

	if (modelObjects["main"].GetAnimationFrame() == 17) FallStart(3.0f);

	if (modelObjects["main"].GetAnimationFrame() == 47) 
	{
		jumpAttackTimer.SetStopFlag(false);

	}

	// ジャンプ攻撃終了処理
	if (modelObjects["main"].GetAnimationEndFlag()) 
	{
		currentAttack = CurrentAttack::NONE;
		modelObjects["main"].SetAnimation("No_Cont");

		actionTimer.SetStopFlag(false);
		actionTimer.ResetTimeZero();
	}
	
}

void Boss::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Boss/Boss.fbx",true,"boss");
}

Boss::Boss()
	:GameObject("boss")
{
	modelObjects["main"].Create(MelLib::ModelData::Get("boss"), "boss", nullptr);
	modelObjects["main"].SetAnimation("No_Cont");


	//modelObjects["main"].SetPosition();
	//sphereDatas["main"].resize(3);


	actionTimer.SetResetFlag(false);



	actionTimer.SetMaxTime(60 * 3);
	jumpAttackTimer.SetMaxTime(60 * 0.3f);
	jumpAttackTimer.SetResetFlag(false);

	// 仮設定
	/*modelObjects["main"].SetAnimation("Attack_Jump.001");
	currentAttack = CurrentAttack::JUMP;*/


	hp.SetData(100, "Boss", "HP", 1, 300);

}

std::shared_ptr<MelLib::GameObject> Boss::GetNewPtr()
{
	return std::make_shared<Boss>();
}

void Boss::Initialize()
{
	FallStart(0.0f);

	// 当たり判定の作成
	capsuleDatas["main"].resize(1);
	capsuleDatas["main"][0].SetRadius(4.5f);
	capsuleDatas["main"][0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(GetPosition() + MelLib::Vector3(0, 25.0f, 0), GetPosition() + MelLib::Vector3(0,-3.0f, 0)));

	segment3DDatas["main"].resize(1);
	segment3DDatas["main"][0].SetPosition(capsuleDatas["main"][0].GetSegment3DData().GetPosition());


	// 仮
	thisState = ThisState::BATTLE;
}

void Boss::Update()
{



	// 向きの計算
	MelLib::Vector2 angle2 = MelLib::LibMath::AngleToVector2(GetAngle().y + 90 , true);
	direction = MelLib::Vector3(angle2.x, 0, angle2.y);
	

	modelObjects["main"].Update();
	modelObjects["main"].SetAnimationPlayFlag(true);

	SelectAction();
	AttackUpdate();
	CalcMovePhysics();
}

void Boss::Draw()
{
	AllDraw();
}

void Boss::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if (typeid(object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{

		MelLib::Vector3 addPos;

		//投げ上げ処理終了
		FallEnd();

		addPos.y += GetSegmentCalcResult().triangleHitPos.y - segment3DDatas["main"][0].GetPosition().v2.y;

		AddPosition(addPos);

	/*	if (currentThisAttackEffect == AttackEffect::BE_BLOWN_AWAY)
		{
			currentThisAttackEffect = AttackEffect::NONE;

			state = ThisState::GET_UP;
			modelObjects["main"].SetAnimation("BeBlownAway_2");
			modelObjects["main"].SetAnimationFrameStart();
		}*/
	}
}
