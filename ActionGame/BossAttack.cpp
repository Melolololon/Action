#include "BossAttack.h"

Player* BossAttack::pPlayer;

void BossAttack::LoadResources()
{
	bool r = MelLib::ModelData::Load("Resources/Model/Boss/BossAttack/BossAttack.obj", false, "BossAttack");
}

BossAttack::BossAttack(const MelLib::Vector3& pos)
	:GameObject("BossAttack")
{
	SetPosition(pos + MelLib::Vector3(0,-7.0f,0));

	modelObjects["main"].Create(MelLib::ModelData::Get("BossAttack"), "BossAttack");
	modelObjects["main"].SetPosition(pos);

	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(pos);
	sphereDatas["main"][0].SetRadius(6.0f);

	deadTimer.SetMaxTime(60 * 0.8f);
	SetScale(10.0f);
	sphereDatas["main"][0].SetRadius(5.0f);
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
	CalcMovePhysics();
}

void BossAttack::Draw()
{
	AllDraw();
}

void BossAttack::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if (typeid(object) == typeid(Player)) 
	{
		pPlayer->DownHP(80);
	}
}
