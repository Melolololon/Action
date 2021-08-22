#include "Ground.h"

#include<ModelData.h>

Ground::Ground(const MelLib::Vector3& pos, const MelLib::Vector3& size,  MelLib::Material* pMaterial)
{
	position = pos;

	//モデル用意
	//板ポリをセットして生成
	model.Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOARD), nullptr);
	model.SetMaterial(pMaterial, 0);
	model.SetAngle(MelLib::Vector3(90.0f, 0.0f, 0.0f));
	model.SetScale(size);
	model.SetPosition(position);

	boardData.resize(1);
	
}

void Ground::Update()
{
}

void Ground::Draw()
{
}

void Ground::Hit(const GameObject* const object, const MelLib::ShapeType3D collisionType, const int arrayNum, const MelLib::ShapeType3D hitObjColType, const int hitObjArrayNum)
{
}
