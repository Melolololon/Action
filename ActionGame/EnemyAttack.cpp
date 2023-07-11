#include "EnemyAttack.h"

Player* EnemyAttack::pPlayer;

#include"EditMode.h"
#include"Pause.h"

const std::string EnemyAttack::ATTACK_TYPE_STR_NORMAL = "normal";
const std::string EnemyAttack::ATTACK_TYPE_STR_BE = "beBlownAway";


EnemyAttack::AttackType EnemyAttack::GetAttackType(const std::string& tag)
{
	if(tag == ATTACK_TYPE_STR_BE)return  AttackType::BE_BLOWN_AWAY;

	return AttackType::NORMAL;
}

std::string EnemyAttack::GetAttackTypeStr(const AttackType type)
{
	if (type == AttackType::BE_BLOWN_AWAY )return ATTACK_TYPE_STR_BE;

	return "";
}

EnemyAttack::EnemyAttack
(
	unsigned int power,
	const MelLib::Vector3& attackStartPos,
	float radius,
	const MelLib::ModelObject& model,
	const MelLib::Vector3& modelStartPos,
	const MelLib::Vector3& modelStartAngle,
	const MelLib::Vector3& modelStartScale,
	const AttackType attackType,
	const unsigned int deleteFrame
) 
	:GameObject("EnemyAttack")
	, SPHERE_START_POSITION(attackStartPos)
	, power(power)
	, MODEL(model)
	, MODEL_START_POS(modelStartPos)
	, MODEL_START_ANGLE(modelStartAngle)
	, MODEL_START_SCALE(modelStartScale)
	, MOVE_TYPE(MoveType::ANIMATION)
	, DELETE_FRAME(deleteFrame)
{
	// アニメーションに合わせてsphereを動かす
	SetInitData(radius, attackType);
}

EnemyAttack::EnemyAttack
(
	unsigned int power, 
	const MelLib::ModelObject& model, 
	float radius, 
	const AttackType attackType,
	const unsigned int deleteFrame
)
	:GameObject("EnemyAttack")
	, power(power)
	, MODEL(model)
	, MOVE_TYPE(MoveType::OBJECT)
	, DELETE_FRAME(deleteFrame)
{
	SetInitData(radius, attackType);

	skipCollisionCheckTags.push_back("Stage");
}

void EnemyAttack::SetAttackTypeTag(AttackType type)
{
	switch (type)
	{
	case EnemyAttack::AttackType::NORMAL:
		tags.push_back(ATTACK_TYPE_STR_NORMAL);
		break;
	case EnemyAttack::AttackType::BE_BLOWN_AWAY:
		tags.push_back(ATTACK_TYPE_STR_BE);
		break;
	default:
		break;
	}
}

void EnemyAttack::SetInitData(const float radius, AttackType type)
{
	sphereDatas["main"].resize(1);
	SetScale(radius * 2);
	SetAttackTypeTag(type);

}

void EnemyAttack::Update()
{
	if (Pause::GetInstance()->GetIsPause())return;

	timer++;

	if (MOVE_TYPE == MoveType::ANIMATION) 
	{
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
	}
	else if (MOVE_TYPE == MoveType::OBJECT) 
	{
		SetPosition(MODEL.GetPosition());
	}

	//if (MODEL.GetAnimationFrameCount() - (MODEL.GetAnimationFrame() - 7))
	//if (MODEL.GetAnimationEndFlag())eraseManager = true;
	//if (timer >= 60 * 0.5)eraseManager = true;

	if (MODEL.GetAnimationFrame() >= DELETE_FRAME)eraseManager = true;
}

void EnemyAttack::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if (typeid(object) == typeid(Player)/*
		&& !pPlayer->GetIsMuteki()*/)
	{
		pPlayer->DownHP(power);
		eraseManager = true;
	}
}
