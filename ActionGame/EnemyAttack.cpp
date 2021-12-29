#include "EnemyAttack.h"

Player* EnemyAttack::pPlayer;

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
	// �A�j���[�V�����ɍ��킹��sphere�𓮂���

	sphereData.resize(1);
	sphereData[0].SetRadius(radius);


	deadTimer.SetMaxTime(deadTime);
	deadTimer.SetStopFlag(false);
}

void EnemyAttack::Update()
{
	// ���W�v�Z���ăZ�b�g
 	sphereData[0].SetPosition(MODEL.CalcAnimationPosition
	(
		SPHERE_START_POSITION,
		1.0f,
		"Bone_L.003",
		"Body.001",
		MODEL_START_POS,
		MODEL_START_ANGLE,
		MODEL_START_SCALE
	));
	//����ƈʒu�����̂܂܃Z�b�g
 	SetPosition(sphereData[0].GetPosition());

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
