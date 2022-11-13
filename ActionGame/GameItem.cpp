#include "GameItem.h"
Player* GameItem::pPlayer;

bool GameItem::CheckHitPlayer(const MelLib::GameObject& pObject)
{

	return typeid(pObject) == typeid(Player);
}

void GameItem::Erase()
{
	// ここにエフェクト追加処理

	eraseManager = true;
}

GameItem::GameItem(const MelLib::Vector3& pos)
	:GameObject("Item")
{
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(pos);
	sphereDatas["main"][0].SetRadius(5);

	modelObjects["main"].SetPosition(pos);

	SetPosition(pos);
}

void GameItem::Update()
{

}

void GameItem::Draw()
{
}

void GameItem::Hit(const MelLib::GameObject&  object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	
}
