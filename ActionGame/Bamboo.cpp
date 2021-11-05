#include "Bamboo.h"
#include"PlayerSlush.h"

Bamboo::Bamboo(const MelLib::Vector3& pos)
	:StageObject(pos,"")
{
}

Bamboo::Bamboo(const MelLib::Vector3& pos, MelLib::ModelData* pCatModelData) 
	: StageObject(pos, pCatModelData)
{
}

void Bamboo::Update()
{
}


void Bamboo::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{

}
