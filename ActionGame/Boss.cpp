#include "Boss.h"


void Boss::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Boss/Boss.fbx",true,"boss");
}

void Boss::SetPlayer()
{
}

Boss::Boss() 
	:GameObject("boss")
{
	modelObjects["main"].Create(MelLib::ModelData::Get("boss"),"boss", nullptr);
	modelObjects["main"].SetAnimation("No_Cont");


	//modelObjects["main"].SetPosition();
	//sphereDatas["main"].resize(3);


	actionTimer.SetMaxTime(60 * 6);
}

void Boss::Update()
{
	if (thisState == ThisState::BATTLE)actionTimer.SetStopFlag(false);
}

void Boss::Draw()
{
	AllDraw();
}

void Boss::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
}


void Boss::Attack()
{
}

void Boss::Move()
{
}
