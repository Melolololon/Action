#include "EnemyAttack.h"

Player* EnemyAttack::pPlayer;

#include"EditMode.h"
#include"Pause.h"

EnemyAttack::EnemyAttack
(
	unsigned int power,
	const MelLib::Vector3& attackStartPos,
	float radius,
	const MelLib::ModelObject& model,
	const MelLib::Vector3& modelStartPos,
	const MelLib::Vector3& modelStartAngle,
	const MelLib::Vector3& modelStartScale
) :
	GameObject("EnemyAttack")
	,SPHERE_START_POSITION(attackStartPos)
	, power(power)
	, MODEL(model)
	, MODEL_START_POS(modelStartPos)
	, MODEL_START_ANGLE(modelStartAngle)
	, MODEL_START_SCALE(modelStartScale)
{
	// アニメーションに合わせてsphereを動かす

	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetRadius(radius);


	
}

void EnemyAttack::Update()
{
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

	// 座標計算してセット
  	sphereDatas["main"][0].SetPosition(MODEL.CalcAnimationPosition
	(
		SPHERE_START_POSITION,
		1.0f,
		"Bone_R.002",
		"Body",
		MODEL_START_POS,
		MODEL_START_ANGLE,
		MODEL_START_SCALE
	));

	if (MODEL.GetAnimationEndFlag())eraseManager = true;
}

void EnemyAttack::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if (typeid(object) == typeid(Player)
		&& !pPlayer->GetIsMuteki())
	{
		pPlayer->DownHP(power);
		eraseManager = true;
	}
}
