#include "EnemyAttack.h"

Player* EnemyAttack::pPlayer;

EnemyAttack::EnemyAttack
(
	unsigned int power,
	const MelLib::Vector3& pos,
	float radius,
	int deadTime,
	const MelLib::ModelObject& model,
	const MelLib::Vector3& startPos,
	const MelLib::Vector3& startAngle,
	const MelLib::Vector3& startScale
) :
	power(power)
	, MODEL(model)
	, START_POS(startPos)
	, START_ANGLE(startAngle)
	, START_SCALE(startScale)
{
	// アニメーションに合わせてsphereを動かす

	sphereData.resize(1);
	sphereData[0].SetPosition(pos);
	sphereData[0].SetRadius(radius);

	SetPosition(pos);

	deadTimer.SetMaxTime(deadTime);
	deadTimer.SetStopFlag(false);
}

void EnemyAttack::Update()
{
	SetPosition(MODEL.CalcAnimationPosition
	(
		GetPosition(),
		1.0f,
		"",
		"",
		START_POS,
		START_ANGLE,
		START_SCALE
	)
	);

	if (deadTimer.GetMaxOverFlag())
	{
		eraseManager = true;
	}
}

void EnemyAttack::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
	if (typeid(*object) == typeid(Player))
	{
		pPlayer->DownHP(power);
	}
}
