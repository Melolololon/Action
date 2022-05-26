#include "Bamboo.h"

#include<GameObjectManager.h>

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


void Bamboo::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	MelLib::ModelData* pFront = nullptr;
	MelLib::ModelData* pBack = nullptr;
	//MeshCat(object, pFront, pBack);
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Bamboo>(GetPosition(), pFront));
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Bamboo>(GetPosition(), pBack));
}
