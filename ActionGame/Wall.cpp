#include "Wall.h"

// これも床みたいにメッシュ通りの判定作ってもいいかも
// モデル表示せずに三角形だけもらう
// ステージに移してもいいかも
// 壁の下の部分だけ分離してメッシュの三角形情報取得すればいい

Wall::Wall(const MelLib::Vector3& pos, const MelLib::Vector3& angle, const MelLib::Vector2& scale)
{
	SetPosition(pos);

	//仮(sttaicなどにしたりして生成一回にしといて)
	/*testMtl.Create(MelLib::PipelineState::GetDefaultDrawData(MelLib::PipelineStateType::MODEL));
	testMtl.SetTexture(MelLib::Texture::Get("ground"));*/

	//モデル用意
	//板ポリをセットして生成
	//modelObjects.emplace("main",MelLib::ModelObject());
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	//modelObjects["main"].SetMaterial(&testMtl);
	/*modelObjects["main"].SetAngle(MelLib::Vector3(90.0f, 0.0f, 0.0f));
	modelObjects["main"].SetScale(GROUND_SIZE.ToVector3());
	modelObjects["main"].SetPosition(GetPosition());*/
	modelObjects["main"].SetAngle(angle);
	modelObjects["main"].SetScale(scale.ToVector3());
	modelObjects["main"].SetPosition(pos);

	//boardData.resize(1);
	//boardData[0].SetAngle(MelLib::Vector3(90, 0, 0));
	//boardData[0].SetSize(GROUND_SIZE);
	//boardData[0].SetPosition(0);

	boardData.resize(1);
	boardData[0].SetAngle(angle);
	boardData[0].SetSize(scale);
	boardData[0].SetPosition(pos);
}

void Wall::Update()
{
}

void Wall::Draw()
{
	//modelObjects["main"].Draw();
}

void Wall::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
}
