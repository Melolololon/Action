#include "Boss.h"

void Boss::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Boss/Boss.fbx",true,"boss");
}

Boss::Boss(const MelLib::Vector3 pos)
{
	modelObjects["main"].Create(MelLib::ModelData::Get("boss"), nullptr);

	modelObjects["main"].SetPosition(pos);
	sphereData.resize(3);

}

void Boss::Update()
{
}

void Boss::Draw()
{
}

void Boss::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
}
