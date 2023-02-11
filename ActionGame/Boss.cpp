#include "Boss.h"


void Boss::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Boss/Boss.fbx",true,"boss");
}

void Boss::SetPlayer()
{
}

Boss::Boss(const MelLib::Vector3 pos) 
	:GameObject("boss")
{
	modelObjects["main"].Create(MelLib::ModelData::Get("boss"),"boss", nullptr);

	modelObjects["main"].SetPosition(pos);
	sphereDatas["main"].resize(3);

}

void Boss::Update()
{
}

void Boss::Draw()
{
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
