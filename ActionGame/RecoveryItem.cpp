#include "RecoveryItem.h"

void RecoveryItem::LoadResources()
{

	MelLib::ModelData::Load("Resources/Model/Item/H_Item/H_Item.obj", false, "RecoveryItem");
}

RecoveryItem::RecoveryItem():
	GameItem()
{
	modelObjects["main"].Create(MelLib::ModelData::Get("RecoveryItem"), "RecoveryItem");
}

void RecoveryItem::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if(CheckHitPlayer(object))
	{
		// ‰ñ•œ
		pPlayer->LifeUp(RECOVERY_HP);
		
		// íœ
		Erase();
	}
}

std::shared_ptr<MelLib::GameObject> RecoveryItem::GetNewPtr()
{
	return std::make_shared<RecoveryItem>();
	//return nullptr;
}
