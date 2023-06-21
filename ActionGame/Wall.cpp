#include "Wall.h"
#include "Game.h"
#include"EnemyDeadCounter.h"
#include"GamePlay.h"

// static変数でカウントして、特定の位置に壁出す?

MelLib::ADSAMaterial Wall::material;
unsigned int Wall::wallSetCount;


void Wall::LoadResources()
{
	// テクスチャ読み込み
	const std::string TEXTURE_PATH = Game::GetInstance()->GetPath(Game::ResourcePath::TEXTURE);

	MelLib::Texture::Load(TEXTURE_PATH + "Wall.png","wall");

	MelLib::DrawOption op;
	op.SetModelDefData();
	material.Create(op,1);
	material.SetTexture(MelLib::Texture::Get("wall"));
}

Wall::Wall()
	:GameObject("Wall")
{
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOARD), "Wall");
	modelObjects["main"].SetMaterial(&material);
	
	wallNum.SetData(1,GetObjectName(), "DeleteNum", 1, 99);

	// 仮
	//wallNum.SetValue(0);

	//wallSpr.Create(MelLib::Texture::Get("wall"));

	/*
	wallSpr.SetAngle(angle);
	wallSpr.SetScale(scale);
	wallSpr.SetPosition(pos);

	boardDatas["main"].resize(1);
	boardDatas["main"][0].SetAngle(angle);
	boardDatas["main"][0].SetSize(scale);
	boardDatas["main"][0].SetPosition(pos);*/

	
	
}

void Wall::Initialize()
{
	boardDatas["main"].resize(1);
	boardDatas["main"][0].SetAngle(GetAngle());
	MelLib::Vector3 scale = GetScale();
	boardDatas["main"][0].SetSize(MelLib::Vector2(scale.x, scale.y));
	boardDatas["main"][0].SetPosition(GetPosition());
}

void Wall::Update()
{
	// 仮
	if (deleteFlag)
	{
		// ボス戦で壁復活
		if (GamePlay::GetState() == GamePlay::GamePlayState::BOSS) 
		{
			deleteFlag = false;
			collisionFlag.board = true;
		}
		return;
	}

	// 画像のスクロール
	static const float AREA_SCROLL_SPEED = 0.01f;

	modelObjects["main"].SetAddUV(MelLib::Vector2(0, modelObjects["main"].GetAddUV().y + AREA_SCROLL_SPEED));

	/*drawAreaLeftUp.y -= AREA_SCROLL_SPEED;
	drawAreaRightDown.y -= AREA_SCROLL_SPEED;*/
	/*wallSpr.SetDrawLeftUpPosition(drawAreaLeftUp);
	wallSpr.SetDrawRigthDownPosition(drawAreaRightDown);*/

	if (wallNum.GetValue() <= EnemyDeadCounter::GetInstance()->GetCount() && GamePlay::GetState() != GamePlay::GamePlayState::BOSS)
	{
		deleteFlag = true;
		collisionFlag.board = false;
	}

}

void Wall::Draw()
{
	if (deleteFlag)return;

	//wallSpr.Draw();
	AllDraw();
}

std::shared_ptr<MelLib::GameObject> Wall::GetNewPtr()
{
	return std::make_shared<Wall>();
}
