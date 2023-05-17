#include "CapsuleEnemyAttack.h"

#include"Pause.h"

Player* CapsuleEnemyAttack::pPlayer;

CapsuleEnemyAttack::CapsuleEnemyAttack
(
	unsigned int power, 
	const MelLib::Value2<MelLib::Vector3>& attackStartPos,
	float radius,
	const MelLib::ModelObject& model,
	const MelLib::Vector3& modelStartPos,
	const MelLib::Vector3& modelStartAngle,
	const MelLib::Vector3& modelStartScale,
	const std::string& boneName,
	const std::string& meshName
)
	:GameObject("CapsuleEnemyAttack")
	, CAPSULE_START_POSITION(attackStartPos)
	, power(power)
	, MODEL(model)
	, MODEL_START_POS(modelStartPos)
	, MODEL_START_ANGLE(modelStartAngle)
	, MODEL_START_SCALE(modelStartScale)
	, BONE_NAME(boneName)
	, MESH_NAME(meshName)
{
	capsuleDatas["main"].resize(1);
	MelLib::Segment3DData d;
	d.SetPosition(attackStartPos);
	capsuleDatas["main"][0].SetSegment3DData(d);
	capsuleDatas["main"][0].SetRadius(radius);

	collisionCheckDistance = 30.0f;

}

void CapsuleEnemyAttack::Update()
{

	//"Bone_R.003",
	//	"Body",

	if (Pause::GetInstance()->GetIsPause())return;

	MelLib::Value2<MelLib::Vector3> pos;

	// 座標計算してセット
	pos.v1 = MODEL.CalcAnimationPosition
	(
		CAPSULE_START_POSITION.v1,
		1.0f,
		BONE_NAME,
		MESH_NAME,
		MODEL_START_POS,
		MODEL_START_ANGLE,
		MODEL_START_SCALE
	);

	pos.v2 = MODEL.CalcAnimationPosition
	(
		CAPSULE_START_POSITION.v2,
		1.0f,
		BONE_NAME,
		MESH_NAME,
		MODEL_START_POS,
		MODEL_START_ANGLE,
		MODEL_START_SCALE
	);


	capsuleDatas["main"][0].GetRefSegment3DData().SetPosition(pos);

	// これないと判定の距離制限のせいで処理無視される
	// 仮にv1の値を設定
	SetPosition(pos.v1);

	if (MODEL.GetAnimationEndFlag())
	{
		eraseManager = true;
	}
}

void CapsuleEnemyAttack::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if (typeid(object) == typeid(Player)
		/*&& !pPlayer->GetIsMuteki()*/)
	{
		pPlayer->DownHP(power);
		eraseManager = true;
	}
}
