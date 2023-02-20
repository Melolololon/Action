#include "BossAttack.h"

void BossAttack::LoadResources()
{
	bool r = MelLib::ModelData::Load("Resources/Model/Boss/BossAttack/BossAttack.obj", true, "BossAttack");
}

BossAttack::BossAttack(const MelLib::Vector3& pos)
	:GameObject("BossAttack")
{
	SetPosition(pos + MelLib::Vector3(0,-5.0f,0));

	modelObjects["main"].Create(MelLib::ModelData::Get("BossAttack"), "BossAttack");
	modelObjects["main"].SetPosition(pos);

	deadTimer.SetMaxTime(60 * 1.2f);
	SetScale(6.0f);
}

void BossAttack::Initialize()
{
	FallStart(1.0f);
	deadTimer.SetStopFlag(false);
}

void BossAttack::Update()
{
	if (deadTimer.GetMaxOverFlag())
	{
		eraseManager = true;
	}

	
	if (deadTimer.GetNowTime() <= 60 * 0.3f || deadTimer.GetNowTime() >= deadTimer.GetMaxTime() - 60 * 0.2f) 
	{
		CalcMovePhysics();
	}
}

void BossAttack::Draw()
{
	AllDraw();
}

void BossAttack::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
}
