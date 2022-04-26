#include "StageObject.h"

#include<GameObjectManager.h>
#include<LibMath.h>

#include"PlayerSlush.h"



Player* StageObject::pPlayer;

MelLib::PlaneData StageObject::SegmentToPlane()
{
	// moveVectorYの角度変換は関数作ったほうがいい
	// (あと、今のやり方じゃダメ。(0.7の時、63度とかになる))

#pragma region テスト用
	MelLib::Vector3 normal(0, 1, 0);
	MelLib::Vector3 moveVector = (MelLib::Vector3(1, 1, 0) - MelLib::Vector3(0, 0, 0)).Normalize();


	// xとzを考慮する場合、どうするか
	// 先にプレイヤーが奥(zの+方向)を向いてると仮定して回転させて、そのあとプレイヤーの向き分回転させる?
	// 向きを角度に変換して、回転させればいけそう?

	// moveVector.yを角度に変換
	float angle = moveVector.y * 90.0f;

	// 法線を回転
	normal = MelLib::LibMath::RotateVector3(normal, MelLib::Vector3(0, 0, -1), angle);

	// プレイヤーの向きを取得
	MelLib::Vector3 playerDirection = MelLib::Vector3(1, 0, 0);
	float playerDirAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(playerDirection.x, playerDirection.z), true);

	// z+側向いてるときは回転させる必要ないから-90;
	playerDirAngle -= 90;

	// 向きに合わせて更に回転
	normal = MelLib::LibMath::RotateVector3(normal, MelLib::Vector3(0, -1, 0), playerDirAngle);

	// 攻撃を元に平面の情報を作成
	MelLib::PlaneData playerSlushPlane;

	// 衝突点を代入
	playerSlushPlane.SetPosition(capsuleDatas[0].GetRefSegment3DData().GetCalcResult().lineSegment3DHitPos);

	playerSlushPlane.SetNormal(normal);

	return playerSlushPlane;
#pragma endregion


	//// Yの移動方向で法線を回転させればいける?

	//PlayerSlush* pPlayerSlush = pPlayer->GetPPlayerSlush();

	//// ここ武器の先端の座標だけでもいいかも
	//MelLib::Value2<MelLib::Vector3>sgmentPosition = pPlayerSlush->GetSegmentPosition();
	//MelLib::Value2<MelLib::Vector3>sgmentPrePosition = pPlayerSlush->GetSegmentPosition();

	//MelLib::Vector3 normal(0, 1, 0);
	//MelLib::Vector3 moveVector = (sgmentPosition.v2 - sgmentPrePosition.v2).Normalize();
	//

	//// xとzを考慮する場合、どうするか
	//// 先にプレイヤーが奥(zの+方向)を向いてると仮定して回転させて、そのあとプレイヤーの向き分回転させる?
	//// 向きを角度に変換して、回転させればいけそう?

	//// moveVector.yを角度に変換
	//float angle = moveVector.y * 90.0f;

	//// 法線を回転
	//normal = MelLib::LibMath::RotateVector3(normal, MelLib::Vector3(0, 0, -1), angle);

	//// プレイヤーの向きを取得
	//MelLib::Vector3 playerDirection = pPlayer->GetDirection();
	//float playerDirAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(playerDirection.x, playerDirection.z), true);

	//// z+側向いてるときは回転させる必要ないから-90;
	//playerDirAngle -= 90;

	//// 向きに合わせて更に回転
	//normal = MelLib::LibMath::RotateVector3(normal, MelLib::Vector3(0, -1, 0), playerDirAngle);

	//// 攻撃を元に平面の情報を作成
	//MelLib::PlaneData playerSlushPlane;

	//// 衝突点を代入
	//playerSlushPlane.SetPosition(capsuleData[0].GetRefSegment3DData().GetCalcResult().lineSegment3DHitPos);

	//playerSlushPlane.SetNormal(normal);

	//return playerSlushPlane;
}

void StageObject::MeshCat(const MelLib::GameObject* const object, MelLib::ModelData*& pFront, MelLib::ModelData*& pBack)
{
	// プレイヤーの武器と接触したら、MeshCat関数を呼び出す
	if (typeid(*object) == typeid(PlayerSlush))
	{
		modelObjects["main"].MeshCat(SegmentToPlane(), pFront, pBack, true);

		// GameObjectの追加は基底クラスでやらないといけない
	}
}

StageObject::StageObject(const MelLib::Vector3& pos, const std::string& modelName)
{

	SetPosition(pos);

	if (modelName == "")
	{
		modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX),nullptr);

		modelObjects["main"].SetScale(3);
	}
	else
	{
		modelObjects["main"].Create(MelLib::ModelData::Get(modelName), nullptr);
	}
	modelObjects["main"].SetPosition(pos);


	// テスト用
	//SegmentToPlane();


	tags.push_back("StageObject");
}

StageObject::StageObject(const MelLib::Vector3& pos, MelLib::ModelData* pCatModelData)
{
	modelObjects["main"].Create(pCatModelData, nullptr);
	modelObjects["main"].SetPosition(pos);
	SetPosition(pos);

	tags.push_back("StageObject");
}

StageObject::~StageObject()
{
}

void StageObject::Update()
{
}

void StageObject::Draw()
{
	modelObjects["main"].Draw();
}

void StageObject::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{	
	
}
