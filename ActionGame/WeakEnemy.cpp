#include "WeakEnemy.h"
#include"Pause.h"
#include"Stage.h"

const float WeakEnemy::MOVE_SPEED = 0.2f;

void WeakEnemy::Attack()
{
	timer++;

	if (timer >= 60 * 1.5f)
	{
		isAttack = false;
		timer = 0;
	}
}

WeakEnemy::WeakEnemy(const MelLib::Vector3& pos):NewEnemy("WeakEnemy")
{
	SetPosition(pos);

	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX),"WeakEnemy");
	modelObjects["main"].SetPosition(pos);

	// 当たり判定の作成
	capsuleDatas["main"].resize(1);
	capsuleDatas["main"][0].SetRadius(4.5f);
	capsuleDatas["main"][0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, -10.0f, 0)));

	segment3DDatas["main"].resize(1);
	segment3DDatas["main"][0].SetPosition(capsuleDatas["main"][0].GetSegment3DData().GetPosition());

	// プレイヤーみたいな十字の線判定作って、壁でジャンプするようにする


	modelObjects["main"].SetScale(3);
}

void WeakEnemy::Initialize()
{
	FallStart(0.0f);
}

void WeakEnemy::Update()
{
	CalcMovePhysics();

	if (isAttack)Attack();
}

void WeakEnemy::Draw()
{
	if (isAttack)modelObjects["main"].SetMulColor(MelLib::Color(0, 0, 255, 255));
	else modelObjects["main"].SetMulColor(MelLib::Color(255,255, 255, 255));
	AllDraw();
}

void WeakEnemy::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	//if (Pause::GetInstance()->GetIsPause())return;

	//if (isDead)return;

	//std::string n = typeid(object).name();
	//if (typeid(object) == typeid(PlayerSlush) && !isMuteki)
	//{

	//	// プレイヤーから現在の攻撃の攻撃力を取得し、体力を減らす
	//	hp -= pPlayer->GetCurrentAttackPower();

	//	// 無敵に
	//	isMuteki = true;
	//	mutekiTimer.SetStopFlag(false);

	//	// 硬直処理
	//	isStun = true;
	//	modelObjects["main"].SetAnimation("Stun");

	//	modelObjects["main"].SetAnimationFrameStart();
	//	modelObjects["main"].SetAnimationEndStopFlag(true);
	//	modelObjects["main"].SetAnimationReversePlayBack(false);

	//	// 攻撃強制終了
	//	isAttack = false;
	//	attackTimer.ResetTimeZero();
	//	attackTimer.SetStopFlag(true);


	//	// 0になったらやられ処理
	//	if (hp <= 0)
	//	{
	//		isDead = true;

	//		modelObjects["main"].SetAnimation("Dead");
	//	}
	//}

	if (typeid(object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{

		MelLib::Vector3 addPos;

		//投げ上げ処理終了
		FallEnd();

		addPos.y += GetSegmentCalcResult().triangleHitPos.y - segment3DDatas["main"][0].GetPosition().v2.y;

		AddPosition(addPos);
	}
}
