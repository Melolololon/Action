#include "GameItem.h"

#include"Stage.h"

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
	SetPosition(pos);
	sphereDatas["main"].resize(1);
	

	//// 球と板の判定がないため、仮にカプセルにしている ステージは三角形だから関係ない
	//// 当たり判定の作成
	//capsuleDatas["main"].resize(1);
	//capsuleDatas["main"][0].SetRadius(4.5f);

	//segment3DDatas["main"].resize(1);
	//segment3DDatas["main"][0].SetPosition(capsuleDatas["main"][0].GetSegment3DData().GetPosition());
}

void GameItem::Initialize()
{
	FallStart(1.0f);
	sphereDatas["main"][0].SetRadius(20);
	sphereDatas["main"][0].SetPosition(GetPosition());
	/*MelLib::Vector3 pos = GetPosition();
	MelLib::Value2<MelLib::Vector3>segmentPos = 
		MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, 0, 0));
	capsuleDatas["main"][0].GetRefSegment3DData().SetPosition(segmentPos);*/
}

void GameItem::Update()
{
	// なんか勝手に当たり判定の半径が小さくなってるからどこで変わってるか探す
	// 今勝手にSetScaleでモデルと判定の大きさが同じになるようになってるから勝手に変わっちゃう
	sphereDatas["main"][0].SetRadius(5);
	CalcMovePhysics();

	SetAngle(GetAngle() + MelLib::Vector3(0, 3, 0));
}

void GameItem::Draw()
{
	AllDraw();
}

void GameItem::Hit(const MelLib::GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	
}

void GameItem::CheckHitGround(const GameObject& object)
{
	if (typeid(object) == typeid(Stage))
	{
		FallEnd();

		sphereDatas["main"][0].SetPosition(GetPosition() + MelLib::Vector3(0,3,0));
	}
}
