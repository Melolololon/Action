#include "EnemyAttack.h"

Player* EnemyAttack::pPlayer;

#include"EditMode.h"
#include"Pause.h"

EnemyAttack::EnemyAttack
(
	unsigned int power,
	const MelLib::Vector3& attackStartPos,
	float radius,
	int deadTime,
	const MelLib::ModelObject& model,
	const MelLib::Vector3& modelStartPos,
	const MelLib::Vector3& modelStartAngle,
	const MelLib::Vector3& modelStartScale
) :
	SPHERE_START_POSITION(attackStartPos)
	, power(power)
	, MODEL(model)
	, MODEL_START_POS(modelStartPos)
	, MODEL_START_ANGLE(modelStartAngle)
	, MODEL_START_SCALE(modelStartScale)
{
	// アニメーションに合わせてsphereを動かす

	sphereDatas.resize(1);
	sphereDatas[0].SetRadius(radius);


	deadTimer.SetMaxTime(deadTime);
	deadTimer.SetStopFlag(false);
}

void EnemyAttack::Update()
{
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

	// 座標計算してセット
  	sphereDatas[0].SetPosition(MODEL.CalcAnimationPosition
	(
		SPHERE_START_POSITION,
		1.0f,
		"Bone_R.002",
		"Body",
		MODEL_START_POS,
		MODEL_START_ANGLE,
		MODEL_START_SCALE
	));

	if (deadTimer.GetMaxOverFlag())
	{
		eraseManager = true;
	}
}

void EnemyAttack::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
	if (typeid(*object) == typeid(Player)
		&& !pPlayer->GetIsMuteki())
	{
		pPlayer->DownHP(power);
		eraseManager = true;
	}
}
