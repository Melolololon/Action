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

GameItem::GameItem()
	:GameObject("Item")
{
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetRadius(5);


}

void GameItem::Update()
{

}

void GameItem::Draw()
{
	AllDraw();
}

void GameItem::Hit(const MelLib::GameObject&  object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	
}
