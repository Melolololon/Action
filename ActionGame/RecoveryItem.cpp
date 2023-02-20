#include "RecoveryItem.h"

void RecoveryItem::LoadResources()
{

	MelLib::ModelData::Load("Resources/Model/Item/H_Item/H_Item.obj", false, "RecoveryItem");
}

RecoveryItem::RecoveryItem(const MelLib::Vector3& pos):
	GameItem(pos)
{
	modelObjects["main"].Create(MelLib::ModelData::Get("RecoveryItem"), "RecoveryItem");
	modelObjects["main"].SetPosition(pos);
}

void RecoveryItem::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	CheckHitGround(object);

	if(CheckHitPlayer(object))
	{
		// çÌèú
		Erase();
	}
}

std::shared_ptr<MelLib::GameObject> RecoveryItem::GetNewPtr()
{
	return std::make_shared<RecoveryItem>(0);
	//return nullptr;
}
