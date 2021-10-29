#include "Bamboo.h"
#include"PlayerSlush.h"

Bamboo::Bamboo(const MelLib::Vector3& pos)
{
	SetPosition(pos);

	modelObjects["main"];
	//modelObjects["main"].Create(MelLib::ModelData::Get(),nullptr);
	//modelObjects["main"].SetPosition(pos);
	
}

Bamboo::Bamboo(const MelLib::Vector3& pos, MelLib::ModelData* pCatModelData)
{
	SetPosition(pos);

	modelObjects["main"];
	modelObjects["main"].Create(pCatModelData, nullptr);
}

void Bamboo::Update()
{
}

void Bamboo::Draw()
{
}

void Bamboo::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
	// プレイヤーの武器と接触したら、MeshCat関数を呼び出す
	if(typeid(*object) == typeid(PlayerSlush))
	{
		//modelObjects["main"].MeshCat();
	}
}
